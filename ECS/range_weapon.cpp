#include "range_weapon.h"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "Stats.hpp"
#include "ProjectileComponent.hpp"
#include <math.h>

Range_Weapon::Range_Weapon(Manager* man)
{
    timer.start();
    timer.setTimeOut(reloadTime);
    manager = man;
    spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "bow.png").string();
    texture = IMG_LoadTexture(Game::renderer,spritePath.c_str());
    srcR.w = width;
    srcR.h = height;
    destR.w = width*3;
    destR.h = height*3;
}

void Range_Weapon::init()
{
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
}

void Range_Weapon::update()
{
    if (is_equiped)
    {
        if (Game::event.type == SDL_KEYDOWN && timer.timedOut())
        {
            switch(Game::event.key.keysym.scancode)
            {
            case
                SDL_SCANCODE_W:
                direction.x = transform->x_direction;
                direction.y = transform->y_direction;
                rangeAttack();
                is_attacking=true;
                break;
            }
            timer.setTimeOut(reloadTime);
        }

        if (is_attacking)
        {
            if (timer.timedOut())
            {
                update_range_weapon();
            }
        }
    }
}

int Range_Weapon::rangeAttack()
{

    if (transform->x_direction == 0)
    {
        if (transform->y_direction == -1)
        {
            CreateArrow(Vector2D(transform->position.x+(transform->width*transform->scale)*0.5,transform->position.y-50),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
        else
        {
            CreateArrow(Vector2D(transform->position.x+(transform->width*transform->scale)*0.5,transform->position.y+(transform->height*transform->scale)+50),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
    }
    if (transform->y_direction == 0)
    {
        if (transform->x_direction == -1)
        {
            CreateArrow(Vector2D(transform->position.x-50,transform->position.y+(transform->height*transform->scale)*0.5),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
        else
        {
            CreateArrow(Vector2D(transform->position.x+(transform->width*transform->scale),transform->position.y+(transform->height*transform->scale)*0.5),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
    }
    if (transform->y_direction==1)
    {
        if (transform->x_direction == 1)
        {
            CreateArrow(Vector2D(transform->position.x+(transform->width*transform->scale)+50,transform->position.y+(transform->height*transform->scale)+50),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
        if (transform->x_direction == -1)
        {
            CreateArrow(Vector2D(transform->position.x-50,transform->position.y+(transform->height*transform->scale)+50),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
    }
    if (transform->y_direction==-1)
    {
        if (transform->x_direction == 1)
        {
            CreateArrow(Vector2D(transform->position.x+(transform->width*transform->scale)+50,transform->position.y-50),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
        if (transform->x_direction == -1)
        {
            CreateArrow(Vector2D(transform->position.x-50,transform->position.y-50),Vector2D(transform->x_direction,transform->y_direction),200,5,"arrow",32,32,3,damage);
        }
    }
    return 0;
}

void Range_Weapon::draw()
{
    if (is_attacking)
    {
        if (!timer.timedOut())
        {
            transform = &(entity->getComponent<TransformComponent>());
            srcR.y = 0;
            frame = static_cast<int>((timer.getTimeOutStart() / speed) % frames);
            index = width*frame;
            srcR.x = index;
            Vector2D entityPos = transform->position;
            int scale = transform->scale;
            if (direction.x == 0)
            {
                if (direction.y == -1)
                {
                    //setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+18*scale+6 - Game::camera.x;
                    destR.y = entityPos.y+13*scale-15 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-90,NULL,SDL_FLIP_NONE);
                }
                else
                {
                    //setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+18*scale+6 - Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,90,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.y == 0)
            {
                if (direction.x == -1)
                {
                    //setPriority(sprite->priority - 1);
                    Vector2D center = Vector2D(entityPos.x+27,entityPos.y+13*scale+29);
                    destR.x = center.x - 14 - Game::camera.x;
                    destR.y = center.y - 27 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_HORIZONTAL);
                }
                else
                {
                    //setPriority(sprite->priority - 1);
                    Vector2D center = Vector2D(entityPos.x+(18+13)*scale+27,entityPos.y+13*scale+29);
                    destR.x = center.x - 14 - Game::camera.x;
                    destR.y = center.y - 27 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == 1)
            {
                if (direction.y == -1)
                {
                    //setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+(13+18)*scale-10-Game::camera.x;
                    destR.y = entityPos.y-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-45,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    //setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+(13+18)*scale-10-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,45,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == -1)
            {
                if (direction.y == -1)
                {
                    //setPriority(sprite->priority - 1);
                    destR.x = entityPos.x-10-Game::camera.x;
                    destR.y = entityPos.y-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-135,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    //setPriority(sprite->priority + 1);
                    destR.x = entityPos.x-10-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,135,NULL,SDL_FLIP_NONE);
                }
            }
        }
    }
}

int Range_Weapon::DoDamage(Stats &entity1)
{
    entity1.SubtractHealth(1);
    return 0;
}

void Range_Weapon::update_range_weapon()
{
    is_attacking = false;
}

void Range_Weapon::CreateArrow(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam)
{
    auto& arrow(manager->addEntity());
    arrow.addComponent<TransformComponent>(pos.x, pos.y, w, h, sc);
    arrow.addComponent<SpriteComponent>(false, id);
    arrow.addComponent<ProjectileComponent>(range,speed,vel,dam);

    arrow.addComponent<ColliderComponent>(id);
    arrow.addComponent<Stats>(0,2);
    arrow.addGroup(Game::groupPlayerProjectiles);
}
