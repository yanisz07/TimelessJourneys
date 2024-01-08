#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "ECS.hpp"
#include "Components.hpp"
#include <string.h>
#include <SDL.h>

extern Manager manager; //not the same as in game.cpp

class WeaponComponent : public Component
{
public:
    Timer timer;
    std::string attack;
    bool melee;
    SDL_Texture *texture;
    int damage = 0;

    WeaponComponent(Manager *man){
        timer.start();
        timer.setTimeOut(reloadTime);
        melee=true;
        std::cout << "Melee Attack" << std::endl;
        manager = man;
    }

    ~WeaponComponent() override = default;

    int equip()
    {
        std::string type = entity->type;
        sprite = &(entity->getComponent<SpriteComponent>());

        std::map<std::string, Action> actions;
        actions = (entity->manager.getAssetManager()->world.Characters[type].Attacks[attack].Actions);
        Animation anim;
        for (auto it = actions.begin(); it != actions.end(); it++)
        {
            anim = Animation(it->second.y_0, 0, it->second.number_frames, 100, it->second.spriteName);
            sprite->animations[it->first] = anim;
        }
        std::cout << "weapon equiped" << std::endl;
        return 0;
    }

    void init() override
    {
        entityTransform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }


    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch(Game::event.key.keysym.scancode)
            {
                case
                SDL_SCANCODE_Q:
                    if (melee){
                        melee=false;
                        std::cout << "Range" << std::endl;
                    }
                    else
                    {
                        melee = true;
                        std::cout << "Melee" << std::endl;
                    }
                    break;
            }
        }
        if (Game::event.type == SDL_KEYDOWN && timer.timedOut())
        {
            switch(Game::event.key.keysym.scancode)
            {
            case
                SDL_SCANCODE_W:
                if(timer.timedOut())
                {
                    if(!melee)
                    {
                        rangeAttack();
                    }
                    else
                    {
                        frontAttack();
                        std::cout << "Melee Attack" << std::endl;
                    }
                }
                break;

            case SDL_SCANCODE_X:
                if (!melee)
                {
                    roundAttack();
                }
                break;
            }
            timer.setTimeOut(reloadTime);

        }
    }

    int rangeAttack()
    {

        if (entityTransform->x_direction == 0)
        {
            if (entityTransform->y_direction == -1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)*0.5,entityTransform->position.y-50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
            else
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)*0.5,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
        }
        if (entityTransform->y_direction == 0)
        {
            if (entityTransform->x_direction == -1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
            else
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale),entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
        }
        if (entityTransform->y_direction==1)
        {
            if (entityTransform->x_direction == 1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)+50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
            if (entityTransform->x_direction == -1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
        }
        if (entityTransform->y_direction==-1)
        {
            if (entityTransform->x_direction == 1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)+50,entityTransform->position.y-50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
            if (entityTransform->x_direction == -1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y-50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true,damage);
            }
        }
        return 0;
    }

    int roundAttack()
    {
        Vector2D goUp = Vector2D(0,-1);
        Vector2D goDown = Vector2D(0,1);
        Vector2D goLeft = Vector2D(-1,0);
        Vector2D goRight = Vector2D(1,0);

        Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)*0.5,entityTransform->position.y-50),
                                       goUp,200,5,"projectile",true,damage);
        Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)*0.5,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),
                                       goDown,200,5,"projectile",true,damage);
        Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),
                                       goLeft,200,5,"projectile",true,damage);
        Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale),entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),
                                       goRight,200,5,"projectile",true,damage);
        return 0;
    }

    int frontAttack()
    {
        //attack implemented below only for player
        Vector2D entityPos = entityTransform->position;
        int scale = entityTransform->scale;
        auto& attackCol(manager->addEntity());
        if (entityTransform->x_direction == 0)
        {
            if (entityTransform->y_direction == -1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(0,-1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+18*scale,entityPos.y+13*scale-15,50,25,100);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Up",false,1);
            }
            else
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(0,1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+18*scale,entityPos.y+(13+19)*scale,50,25,100);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Down",false,1);
            }
        }
        if (entityTransform->y_direction == 0)
        {
            if (entityTransform->x_direction == -1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(-1,0);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+18*scale-20,entityPos.y+13*scale,25,50,100);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Right",true,1);
            }
            else
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(1,0);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+(18+13)*scale,entityPos.y+13*scale,25,50,100);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Right",false,1);
            }
        }
        if (entityTransform->x_direction == 1)
        {
            if (entityTransform->y_direction == -1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(1,-1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+100,entityPos.y-20,128,32,100,45);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Right",false,1);
            }
            if (entityTransform->y_direction == 1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(1,1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+100,entityPos.y+100,128,32,100,135);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Right",false,1);
            }
        }
        if (entityTransform->x_direction == -1)
        {
            if (entityTransform->y_direction == -1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(-1,-1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x-100,entityPos.y-20,128,32,100,135);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Right",true,1);
            }
            if (entityTransform->y_direction == 1)
            {
                std::cout << "Melee attack" << std::endl;
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(-1,1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x-100,entityPos.y+100,128,32,100,45);
                attackCol.addGroup(Game::groupPlayerAttack);

                sprite->Play("Attack_Right",true,1);
            }
        }
        return 0;
    }

    void draw() override
    {
        if(sprite->currentAction == "Attack_Down")
        {
            std::cout<< "Draw attack" << std::endl;
            float handLocation[3][2] = {{20.0/48.0,26.0/48.0},{29.0/48.0,27.0/48.0},{29.0/48.0,27.0/48.0}};
            //SDL_Rect srcRect = {0,0,380,870};
            entityTransform = &(entity->getComponent<TransformComponent>());
            SDL_Rect destRect;
            destRect.h = (1.0/4.0)*entityTransform->height*entityTransform->scale;
            destRect.w = (2.0/3.0)*destRect.h;
            if(sprite->frame != 3)
            {
                SDL_Point center = {8,48};
                destRect.x = (entityTransform->position.x) -  Game::camera.x +
                             (entityTransform->width * entityTransform->scale) * handLocation[sprite->frame][0]-destRect.w/2;
                destRect.y = (entityTransform->position.y) -  Game::camera.y +
                             (entityTransform->height * entityTransform->scale)* handLocation[sprite->frame][1]-destRect.h*0.95;
                if(sprite->frame == 0)
                {
                    SDL_RenderCopyEx(Game::renderer,texture,NULL,&destRect,-30,&center,SDL_FLIP_NONE);
                }
                else if(sprite->frame == 1)
                {
                    SDL_RenderCopyEx(Game::renderer,texture,NULL,&destRect,90,&center,SDL_FLIP_NONE);
                }
                else if(sprite->frame == 2)
                {
                    SDL_RenderCopyEx(Game::renderer,texture,NULL,&destRect,60,&center,SDL_FLIP_NONE);
                }
            }
        }
    }
private:
    std::string name = "Standard weapon ";

    TransformComponent* entityTransform;
    SpriteComponent* sprite;

    Uint32 reloadTime = 400;
    Manager* manager;

};

#endif // WEAPONCOMPONENT_H
