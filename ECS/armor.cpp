#include "armor.h"
#include "SpriteComponent.hpp"

Armor::Armor()
{
    type="";
    armor=0;
}

void Armor::init()
{
    sprite = &entity->getComponent<SpriteComponent>();
}

std::string Armor::get_type()
{
    return type;
}

void Armor::set_type(std::string t)
{
    type=t;
}

/*void Armor::update()
{
    if (Game::event.type == SDL_KEYDOWN)
    {
        switch (Game::event.key.keysym.sym)
        {
        case SDLK_s :
            if (type == "Silver")
            {
                type = "";
                std::string ToRemove = "Silver";
                size_t pos = sprite->currentAction.find(ToRemove);
                if (pos != std::string::npos) {
                    sprite->currentAction.erase(pos, ToRemove.length());
                }
                if (sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
                {
                    sprite->Play(sprite->currentAction.c_str(),true);
                }
                else
                {
                    sprite->Play(sprite->currentAction.c_str());
                }
            }
            else
            {
                type = "Silver";
                if (sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
                {
                    sprite->Play(sprite->currentAction.c_str(),true);
                }
                else
                {
                    sprite->Play(sprite->currentAction.c_str());
                }
            }
            break;
        default:
            break;
        }
    }
}*/
