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
    WeaponComponent(Manager *man){
        timer.start();
        timer.setTimeOut(reloadTime);
        melee=true;
        std::cout << "Melee Attack" << std::endl;
        manager = man;
    }
    void getTransformComponent()
    {
        entityTransform = &entity->getComponent<TransformComponent>();
    }

    ~WeaponComponent() override = default;

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch(Game::event.key.keysym.sym)
            {
                case SDLK_a:
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
            switch(Game::event.key.keysym.sym)
            {
            case SDLK_z:
                if(timer.timedOut())
                if(!melee)
                {
                    rangeAttack();
                    entity->getComponent<SpriteComponent>().Play("Attack_1");
                }
                else
                {
                    frontAttack();
                    std::cout << "Melee Attack" << std::endl;
                    entity->getComponent<SpriteComponent>().Play("Attack_2");
                }
                break;
            case SDLK_x:
                if (!melee)
                {
                    roundAttack();
                    entity->getComponent<SpriteComponent>().Play("Attack_3");
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
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)*0.5,entityTransform->position.y-50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
            }
            else
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)*0.5,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
            }
        }
        if (entityTransform->y_direction == 0)
        {
            if (entityTransform->x_direction == -1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
            }
            else
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale),entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
            }
        }
        if (entityTransform->y_direction==1)
        {
            if (entityTransform->x_direction == 1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)+50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
            }
            if (entityTransform->x_direction == -1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
            }
        }
        if (entityTransform->y_direction==-1)
        {
            if (entityTransform->x_direction == 1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)+50,entityTransform->position.y-50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
            }
            if (entityTransform->x_direction == -1)
            {
                Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y-50),Vector2D(entityTransform->x_direction,entityTransform->y_direction),200,5,"projectile",true);
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
                                       goUp,200,5,"projectile",true);
        Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale)*0.5,entityTransform->position.y+(entityTransform->height*entityTransform->scale)+50),
                                       goDown,200,5,"projectile",true);
        Game::assets->CreateProjectile(Vector2D(entityTransform->position.x-50,entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),
                                       goLeft,200,5,"projectile",true);
        Game::assets->CreateProjectile(Vector2D(entityTransform->position.x+(entityTransform->width*entityTransform->scale),entityTransform->position.y+(entityTransform->height*entityTransform->scale)*0.5),
                                       goRight,200,5,"projectile",true);
        return 0;
    }

    int frontAttack()
    {
        Vector2D entityPos = entityTransform->position;
        auto& attackCol(manager->addEntity());
        if (entityTransform->x_direction == 0)
        {
            if (entityTransform->y_direction == -1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(0,-1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x,entityPos.y-32,128,32,100);
                attackCol.addGroup(Game::groupPlayerAttack);
            }
            else
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(0,1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x,entityPos.y+128,128,32,100);
                attackCol.addGroup(Game::groupPlayerAttack);
            }
        }
        if (entityTransform->y_direction == 0)
        {
            if (entityTransform->x_direction == -1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(-1,0);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x-32,entityPos.y,32,128,100);
                attackCol.addGroup(Game::groupPlayerAttack);
            }
            else
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(1,0);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+128,entityPos.y,32,128,100);
                attackCol.addGroup(Game::groupPlayerAttack);
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
            }
            if (entityTransform->y_direction == 1)
            {
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(1,1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+100,entityPos.y+100,128,32,100,135);
                attackCol.addGroup(Game::groupPlayerAttack);
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
            }
            if (entityTransform->y_direction == 1)
            {
                std::cout << "Melee attack" << std::endl;
                attackCol.addComponent<TransformComponent>();
                attackCol.getComponent<TransformComponent>().set_directions(-1,1);
                attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x-100,entityPos.y+100,128,32,100,45);
                attackCol.addGroup(Game::groupPlayerAttack);
            }
        }
        return 0;
    }

private:
    std::string name = "Standard weapon ";
    int damage = 0;
    TransformComponent* entityTransform;
    Uint32 reloadTime = 1000;
    bool melee;
    Manager* manager;
};

#endif // WEAPONCOMPONENT_H
