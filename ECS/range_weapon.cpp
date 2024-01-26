#include "range_weapon.hpp"
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

    spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "weaponAssets"/ "bow.png").string();
    std::string bow_effect_path = (projectDir / ".." / "TimelessJourneys" / "assets" /"soundAssets"/ "bow_effect1.mp3").string();
    bowSound = Mix_LoadWAV(bow_effect_path.c_str());

    texture = IMG_LoadTexture(Game::renderer,spritePath.c_str());
    srcR.w = animation.width;
    srcR.h = animation.height;
    destR.w = animation.width*3;
    destR.h = animation.height*3;
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
    Vector2D entityPos = transform->position;
    Vector2D direction = Vector2D(transform->x_direction,transform->y_direction);
    int scale = transform->scale;
    if (direction.x == 0)
    {
        if (direction.y == -1)
        {
            CreateArrow(Vector2D(entityPos.x+(18)*scale+3,entityPos.y+6*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),-90);
        }
        else
        {
            CreateArrow(Vector2D(entityPos.x+(18)*scale+3,entityPos.y+(13+19-8)*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),90);
        }
    }
    if (direction.y == 0)
    {
        if (direction.x == -1)
        {
            CreateArrow(Vector2D(entityPos.x+10*scale,entityPos.y+(13+6)*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),180);
        }
        else
        {
            CreateArrow(Vector2D(entityPos.x+(18+13-4)*scale,entityPos.y+(13+6)*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),0);
        }
    }
    if (direction.x==1)
    {
        if (direction.y == -1)
        {
            CreateArrow(Vector2D(entityPos.x+(13+18-7)*scale,entityPos.y+(13-3)*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),-45);
        }
        if (direction.y == 1)
        {
            CreateArrow(Vector2D(entityPos.x+(13+18-7)*scale,entityPos.y+(13+19-3)*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),45);
        }
    }
    if (direction.x==-1)
    {
        if (direction.y == -1)
        {
            CreateArrow(Vector2D(entityPos.x+10*scale,entityPos.y+10*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),-135);
        }
        if (direction.y == 1)
        {
            CreateArrow(Vector2D(entityPos.x+10*scale,entityPos.y+(13+19-4)*scale),Vector2D(direction.x,direction.y),range,speed,"arrow",15,7,2,damage*entity->getComponent<Stats>().get_damage_mult(),135);
        }
    }
    Mix_PlayChannel(-1,bowSound, 0);
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
            int frame;
            frame = static_cast<int>((timer.getTimeOutStart() / animation.speed) % animation.frames);
            animation.index = animation.width*frame;
            srcR.x = animation.index;
            Vector2D entityPos = transform->position;
            int scale = transform->scale;
            if (direction.x == 0)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+16*scale - Game::camera.x;
                    destR.y = entityPos.y+3*scale - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-90,NULL,SDL_FLIP_NONE);
                }
                else
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+16*scale - Game::camera.x;
                    destR.y = entityPos.y+(13+19-8)*scale - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,90,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.y == 0)
            {
                if (direction.x == -1)
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+6*scale - Game::camera.x;
                    destR.y = entityPos.y+(13+2)*scale - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_HORIZONTAL);
                }
                else
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+(18+13-4)*scale - Game::camera.x;
                    destR.y = entityPos.y+(13+2)*scale - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == 1)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+(13+18-8)*scale-Game::camera.x;
                    destR.y = entityPos.y+(13-8)*scale-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-45,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+(13+18-8)*scale-Game::camera.x;
                    destR.y = entityPos.y+(13+19-8)*scale-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,45,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == -1)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+10*scale-Game::camera.x;
                    destR.y = entityPos.y+5*scale-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-135,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+10*scale-Game::camera.x;
                    destR.y = entityPos.y+(13+19-8)*scale-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,135,NULL,SDL_FLIP_NONE);
                }
            }
        }
    }
}

void Range_Weapon::update_range_weapon()
{
    is_attacking = false;
}

void Range_Weapon::CreateArrow(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam,double angle)
{
    auto& arrow(manager->addEntity());
    arrow.addComponent<TransformComponent>(pos.x, pos.y, h, w, sc);
    arrow.addComponent<SpriteComponent>(id);
    arrow.getComponent<SpriteComponent>().setAngle(angle);
    arrow.addComponent<ProjectileComponent>(range,speed,vel,dam);
    arrow.addComponent<ColliderComponent>(id,pos.x,pos.y,w,h);
    arrow.getComponent<ColliderComponent>().SetAngle(angle);
    arrow.addGroup(Game::groupPlayerProjectiles);
}
