#include "KeyboardController.hpp"
#include "../game.hpp"
#include "sword.h"
#include "range_weapon.h"

void KeyboardController::init()
{
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
    armor = &entity->getComponent<Armor>();
}

void KeyboardController::update()
{
    if(!Game::inventory->get_visibility() and !Game::chestScreen1->isCurrentlyVisible() and !Game::chestScreen2->isCurrentlyVisible())
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
                    if(sprite->currentAction != "Run_Right" + armor->get_type())
                    {
                        sprite->Play("Run_Right" + armor->get_type());
                    }
                }
                else if (pressed_left)
                {
                    transform->x_direction = -1;
                    if(sprite->currentAction != "Run_Right" + armor->get_type() && sprite->spriteFlip==SDL_FLIP_NONE)
                    {
                        sprite->Play("Run_Right" + armor->get_type(),true);
                    }
                    if(sprite->currentAction != "Run_Right" + armor->get_type())
                    {
                        sprite->Play("Run_Right" + armor->get_type(),true);
                    }
                }
                else
                {
                    transform->x_direction = 0;
                    if(sprite->currentAction != "Run_Up" + armor->get_type())
                    {
                        sprite->Play("Run_Up" + armor->get_type());
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
                    if(sprite->currentAction != "Run_Right" + armor->get_type())
                    {
                        sprite->Play("Run_Right" + armor->get_type());
                    }
                }

                else if (pressed_left)
                {
                    transform->x_direction = -1;
                    if(sprite->currentAction != "Run_Right" + armor->get_type() && sprite->spriteFlip==SDL_FLIP_NONE)
                    {
                        sprite->Play("Run_Right" + armor->get_type(),true);
                    }
                    if(sprite->currentAction != "Run_Right" + armor->get_type())
                    {
                        sprite->Play("Run_Right" + armor->get_type(),true);
                    }
                }

                else
                {
                    transform->x_direction = 0;
                    if(sprite->currentAction != "Run_Down" + armor->get_type())
                    {
                        sprite->Play("Run_Down" + armor->get_type());
                    }
                }
                break;
            case SDLK_LEFT:
                pressed_left = true;
                transform->velocity.x = -1;
                if((sprite->currentAction != "Run_Right" + armor->get_type()) && sprite->spriteFlip==SDL_FLIP_NONE)
                {
                    sprite->Play("Run_Right" + armor->get_type(),true);
                }
                if(sprite->currentAction != "Run_Right" + armor->get_type())
                {
                    sprite->Play("Run_Right" + armor->get_type(),true);
                }
                transform->x_direction = -1;
                if (pressed_up) {transform->y_direction = -1;}
                else if (pressed_down) {transform->y_direction = 1;}
                else {transform->y_direction = 0;}
                break;
            case SDLK_RIGHT:
                pressed_right = true;
                transform->velocity.x = 1;

                if(sprite->currentAction != "Run_Right" + armor->get_type())
                {
                    sprite->Play("Run_Right" + armor->get_type());
                }

                //update direction
                transform->x_direction = 1;
                if (pressed_up) {transform->y_direction = -1;}
                else if (pressed_down) {transform->y_direction = 1;}
                else {transform->y_direction = 0;}
                break;
            case SDLK_s:
                if (armor->get_type()=="")
                {
                    armor->set_type("Silver");
                    if (sprite->spriteFlip==SDL_FLIP_HORIZONTAL)
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),true,-1,100,true);
                    }
                    else
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),false,-1,100,true);
                    }
                }
                else if (armor->get_type()=="Silver")
                {
                    armor->set_type("Gold");
                    // Find the position of the substring "Silver"
                    size_t pos = sprite->currentAction.find("Silver");
                    // Check if the substring was found
                    if (pos != std::string::npos) {
                        // Erase the substring from the original string
                        sprite->currentAction.erase(pos, 6);  // Assuming "Silver" is 6 characters long
                    }
                    if (sprite->spriteFlip==SDL_FLIP_HORIZONTAL)
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),true,-1,100,true);
                    }
                    else
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),false,-1,100,true);
                    }
                }
                else if (armor->get_type()=="Gold")
                {
                    armor->set_type("Diamond");
                    // Find the position of the substring "Silver"
                    size_t pos = sprite->currentAction.find("Gold");
                    // Check if the substring was found
                    if (pos != std::string::npos) {
                        // Erase the substring from the original string
                        sprite->currentAction.erase(pos, 4);  // Assuming "Silver" is 6 characters long
                    }
                    if (sprite->spriteFlip==SDL_FLIP_HORIZONTAL)
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),true);
                    }
                    else
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),false,-1,100,true);
                    }
                }
                else if(armor->get_type()=="Diamond")
                {
                    armor->set_type("");
                    // Find the position of the substring "Silver"
                    size_t pos = sprite->currentAction.find("Diamond");
                    // Check if the substring was found
                    if (pos != std::string::npos) {
                        // Erase the substring from the original string
                        sprite->currentAction.erase(pos, 7);  // Assuming "Silver" is 6 characters long
                    }
                    if (sprite->spriteFlip==SDL_FLIP_HORIZONTAL)
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),true,-1,100,true);
                    }
                    else
                    {
                        sprite->Play(sprite->currentAction + armor->get_type(),false,-1,100,true);
                    }
                }
                break;
            default:
                break;
            }
            switch(Game::event.key.keysym.scancode)
            {
            case
                SDL_SCANCODE_Q:
                if (entity->getComponent<Sword>().is_equiped)
                {
                    std::cout << "Range Weapon" << std::endl;
                    entity->getComponent<Sword>().is_equiped=false;
                    entity->getComponent<Range_Weapon>().is_equiped=true;
                }
                else
                {
                    std::cout << "Melee Weapon" << std::endl;
                    entity->getComponent<Sword>().is_equiped=true;
                    entity->getComponent<Range_Weapon>().is_equiped=false;
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
                    if (!pressed_right && !pressed_left)
                    {
                        transform->y_direction=-1;
                        sprite->Play("Idle_Up" + armor->get_type());
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
                        sprite->Play("Idle_Down" + armor->get_type());
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
                        sprite->Play("Idle_Right" + armor->get_type(),true);
                    }
                    else if (pressed_up)
                    {
                        sprite->Play("Run_Up" + armor->get_type());
                        transform->x_direction=0;
                    }
                    else if (pressed_down)
                    {
                        sprite->Play("Run_Down" + armor->get_type());
                        transform->x_direction=0;
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
                        sprite->Play("Idle_Right" + armor->get_type());
                    }
                    else if (pressed_up)
                    {
                        sprite->Play("Run_Up" + armor->get_type());
                        transform->x_direction=0;
                    }
                    else if (pressed_down)
                    {
                        sprite->Play("Run_Down" + armor->get_type());
                        transform->x_direction=0;
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

    transform->velocity.Normalize();

}
