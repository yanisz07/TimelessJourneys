#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "ECS.hpp"
#include "Components.hpp"
#include <string.h>
#include <SDL.h>

class WeaponComponent : public Component
{
public:
    Timer timer;
    WeaponComponent(){timer.start(); timer.setTimeOut(reloadTime);}
    void getTransformComponent()
    {
        entityTransform = &entity->getComponent<TransformComponent>();
    }

    ~WeaponComponent() override = default;

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN && timer.timedOut())
        {
            switch(Game::event.key.keysym.sym)
            {
            case SDLK_z:
                frontAttack();
                entity->getComponent<SpriteComponent>().Play("Attack_1");
                break;
            case SDLK_x:
                roundAttack();
                entity->getComponent<SpriteComponent>().Play("Attack_3");
                break;
            }
            timer.setTimeOut(reloadTime);

        }
    }

    int frontAttack()
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

private:
    std::string name = "Standard weapon ";
    int damage = 0;
    TransformComponent* entityTransform;
    Uint32 reloadTime = 1000;
};

#endif // WEAPONCOMPONENT_H
