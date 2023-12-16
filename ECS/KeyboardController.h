#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
    TransformComponent* transform;

    SpriteComponent* sprite;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_UP:
                transform->velocity.y = -1;
                if(sprite->currentAction != "Run")
                {
                    sprite->Play("Run");
                }

                break;
            case SDLK_DOWN:
                transform->velocity.y = 1;
                if(sprite->currentAction != "Run")
                {
                    sprite->Play("Run");
                }
                break;
            case SDLK_LEFT:
                transform->velocity.x = -1;
                if(sprite->currentAction != "Run")
                {
                    sprite->Play("Run");
                }
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL; //flips on the x axis
                break;
            case SDLK_RIGHT:
                transform->velocity.x = 1;
                if(sprite->currentAction != "Run")
                {
                    sprite->Play("Run");
                }
                break;
            case SDLK_SPACE:
                Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale),transform->position.y),Vector2D(1,0),200,2,"projectile");
                break;
            case SDLK_z:
                if(entity->hasComponent<WeaponComponent>())
                {
                    std::cout << "attack" << std::endl;
                }
            default:
                break;
            }
        }
        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_UP:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_DOWN:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_LEFT:
                transform->velocity.x = 0;
                sprite->Play("Idle");
                sprite->spriteFlip = SDL_FLIP_NONE; //resets horizontal flipping
                break;
            case SDLK_RIGHT:
                transform->velocity.x = 0;
                sprite->Play("Idle");
                break;
            case SDLK_ESCAPE :
                Game::isRunning = false;
            default:
                break;
            }
        }
    }
};

#endif // KEYBOARDCONTROLLER_H
