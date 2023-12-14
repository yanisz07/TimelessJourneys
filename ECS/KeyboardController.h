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
                sprite->Play("Walk");
                //update direction
                transform->x_direction = 0;
                transform->y_direction = -1;
                break;
            case SDLK_DOWN:
                transform->velocity.y = 1;
                sprite->Play("Walk");
                //update direction
                transform->x_direction = 0;
                transform->y_direction = 1;
                break;
            case SDLK_LEFT:
                transform->velocity.x = -1;
                sprite->Play("Walk");
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL; //flips on the x axis
                //update direction
                transform->x_direction = -1;
                transform->y_direction = 0;
                break;
            case SDLK_RIGHT:
                transform->velocity.x = 1;
                sprite->Play("Walk");
                //update direction
                transform->x_direction = 1;
                transform->y_direction = 0;
                break;
            case SDLK_SPACE:
                if (transform->x_direction == 0)
                {
                    if (transform->y_direction == -1)
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale)*0.5,transform->position.y-50),Vector2D(transform->x_direction,transform->y_direction),200,3,"projectile");
                    }
                }
                else
                {
                    Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale)+20,transform->position.y-20),Vector2D(transform->x_direction,transform->y_direction),200,3,"projectile");
                }
                break;
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
