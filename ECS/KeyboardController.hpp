#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component
{
public:
    TransformComponent* transform;

    SpriteComponent* sprite;

    bool pressed_right = false;
    bool pressed_left = false;
    bool pressed_up = false;
    bool pressed_down = false;

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
                pressed_up = true;
                transform->velocity.y = -1;

                //update direction
                transform->y_direction = -1;
                if (pressed_right)
                {
                    transform->x_direction = 1;
                    if(sprite->currentAction != "Run_Right")
                    {
                        sprite->Play("Run_Right");
                    }
                }
                else if (pressed_left)
                {
                    transform->x_direction = -1;
                    if(sprite->currentAction != "Run_Right" && sprite->spriteFlip==SDL_FLIP_NONE)
                    {
                        sprite->Play("Run_Right",true);
                    }
                    if(sprite->currentAction != "Run_Right")
                    {
                        sprite->Play("Run_Right",true);
                    }
                }
                else
                {
                    transform->x_direction = 0;
                    if(sprite->currentAction != "Run_Up")
                    {
                        sprite->Play("Run_Up");
                    }
                }
                break;
            case SDLK_DOWN:
                pressed_down = true;
                transform->velocity.y = 1;

                //update direction
                transform->y_direction = 1;
                if (pressed_right)
                {
                    transform->x_direction = 1;
                    if(sprite->currentAction != "Run_Right")
                    {
                        sprite->Play("Run_Right");
                    }
                }

                else if (pressed_left)
                {
                    transform->x_direction = -1;
                    if(sprite->currentAction != "Run_Right" && sprite->spriteFlip==SDL_FLIP_NONE)
                    {
                        sprite->Play("Run_Right",true);
                    }
                    if(sprite->currentAction != "Run_Right")
                    {
                        sprite->Play("Run_Right",true);
                    }
                }

                else
                {
                    transform->x_direction = 0;
                    if(sprite->currentAction != "Run_Down")
                    {
                        sprite->Play("Run_Down");
                    }
                }
                break;
            case SDLK_LEFT:
                pressed_left = true;
                transform->velocity.x = -1;
                if(sprite->currentAction != "Run_Right" && sprite->spriteFlip==SDL_FLIP_NONE)
                {
                    sprite->Play("Run_Right",true);
                }
                if(sprite->currentAction != "Run_Right")
                {
                    sprite->Play("Run_Right",true);
                }
                transform->x_direction = -1;
                if (pressed_up) {transform->y_direction = -1;}
                else if (pressed_down) {transform->y_direction = 1;}
                else {transform->y_direction = 0;}
                break;
            case SDLK_RIGHT:
                pressed_right = true;
                transform->velocity.x = 1;

                if(sprite->currentAction != "Run_Right")
                {
                    sprite->Play("Run_Right");
                }

                //update direction
                transform->x_direction = 1;
                if (pressed_up) {transform->y_direction = -1;}
                else if (pressed_down) {transform->y_direction = 1;}
                else {transform->y_direction = 0;}
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
                pressed_up = false;
                transform->velocity.y = 0;
                //change direction
                if (!pressed_right && !pressed_left)
                {
                    transform->y_direction=-1;
                    sprite->Play("Idle_Up");
                }
                else
                {
                    transform->y_direction =0;
                }
                break;
            case SDLK_DOWN:
                pressed_down = false;
                transform->velocity.y = 0;
                //change direction
                if (!pressed_right && !pressed_left)
                {
                    transform->y_direction=1;
                    sprite->Play("Idle_Down");
                }
                else
                {
                    transform->y_direction =0;
                }
                break;
            case SDLK_LEFT:
                pressed_left = false;
                transform->velocity.x = 0;
                //change direction
                if (!pressed_up && !pressed_down)
                {
                    transform->x_direction =-1;
                    sprite->Play("Idle_Right",true);
                }
                else
                {
                    transform->x_direction =0;
                }
                break;
            case SDLK_RIGHT:
                pressed_right = false;
                transform->velocity.x = 0;
                //change direction
                if (!pressed_up && !pressed_down)
                {
                    transform->x_direction =1;
                    sprite->Play("Idle_Right");
                }
                else
                {
                    transform->x_direction =0;
                }
                break;

            default:
                break;
            }
        }
    }
};

#endif // KEYBOARDCONTROLLER_H
