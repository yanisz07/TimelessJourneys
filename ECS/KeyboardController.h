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
                sprite->Play("Run");
                //update direction
                transform->y_direction = -1;
                if (pressed_right) {transform->x_direction = 1;}
                else if (pressed_left) {transform->x_direction = -1;}
                else {transform->x_direction = 0;}
                break;
            case SDLK_DOWN:
                pressed_down = true;
                transform->velocity.y = 1;
                sprite->Play("Run");
                //update direction
                transform->y_direction = 1;
                if (pressed_right) {transform->x_direction = 1;}
                else if (pressed_left) {transform->x_direction = -1;}
                else {transform->x_direction = 0;}
                break;
            case SDLK_LEFT:
                pressed_left = true;
                transform->velocity.x = -1;
                sprite->Play("Run");
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL; //flips on the x axis
                //update direction
                transform->x_direction = -1;
                if (pressed_up) {transform->y_direction = -1;}
                else if (pressed_down) {transform->y_direction = 1;}
                else {transform->y_direction = 0;}
                break;
            case SDLK_RIGHT:
                pressed_right = true;
                transform->velocity.x = 1;
                sprite->Play("Run");
                //update direction
                transform->x_direction = 1;
                if (pressed_up) {transform->y_direction = -1;}
                else if (pressed_down) {transform->y_direction = 1;}
                else {transform->y_direction = 0;}
                break;
            case SDLK_SPACE:
                if (transform->x_direction == 0)
                {
                    if (transform->y_direction == -1)
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale)*0.5,transform->position.y-50),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
                    else
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale)*0.5,transform->position.y+(transform->height*transform->scale)+50),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
                }
                if (transform->y_direction == 0)
                {
                    if (transform->x_direction == -1)
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x-50,transform->position.y+(transform->height*transform->scale)*0.5),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
                    else
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale),transform->position.y+(transform->height*transform->scale)*0.5),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
                }
                if (transform->y_direction==1)
                {
                    if (transform->x_direction == 1)
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale)+50,transform->position.y+(transform->height*transform->scale)+50),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
                    if (transform->x_direction == -1)
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x-50,transform->position.y+(transform->height*transform->scale)+50),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
                }
                if (transform->y_direction==-1)
                {
                    if (transform->x_direction == 1)
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x+(transform->width*transform->scale)+50,transform->position.y-50),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
                    if (transform->x_direction == -1)
                    {
                        Game::assets->CreateProjectile(Vector2D(transform->position.x-50,transform->position.y-50),Vector2D(transform->x_direction,transform->y_direction),200,5,"projectile");
                    }
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
                pressed_up = false;
                transform->velocity.y = 0;
                //change direction
                if (!pressed_right && !pressed_left) {transform->y_direction=-1;}
                else {transform->y_direction =0;}
                sprite->Play("Idle");
                break;
            case SDLK_DOWN:
                pressed_down = false;
                transform->velocity.y = 0;
                //change direction
                if (!pressed_right && !pressed_left) {transform->y_direction=1;}
                else {transform->y_direction =0;}
                sprite->Play("Idle");
                break;
            case SDLK_LEFT:
                pressed_left = false;
                transform->velocity.x = 0;
                //change direction
                if (!pressed_up && !pressed_down) {transform->x_direction =-1;}
                else {transform->x_direction =0;}
                sprite->Play("Idle");
                sprite->spriteFlip = SDL_FLIP_NONE; //resets horizontal flipping
                break;
            case SDLK_RIGHT:
                pressed_right = false;
                transform->velocity.x = 0;
                //change direction
                if (!pressed_up && !pressed_down) {transform->x_direction =1;}
                else {transform->x_direction =0;}
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
