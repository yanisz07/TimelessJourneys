#include "Stats.hpp"
#include <SDL.h>
#include "TransformComponent.hpp"
#include "../game.hpp"
#include "sword.h"
#include "range_weapon.h"


void Stats::SubtractHealth(int i)
{
    health -= i;
    if (player) {
        if (health > max_health) {
            health = max_health;
        } else if (health < 0) {
        }
    }
}

void Stats::init()
{
    std::string font = "arial";
    std::string text;
    SDL_Colour color = {0,0,0,255};
    //Draw healthLabel
    text = "Health";
    SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont("arial"),text.c_str(),color);
    healthLabel = SDL_CreateTextureFromSurface(Game::renderer, surf);
    SDL_FreeSurface(surf);
    //Draw expLabel
    text = "Exp";
    surf = TTF_RenderText_Blended(Game::assets->GetFont("arial"),text.c_str(),color);
    expLabel = SDL_CreateTextureFromSurface(Game::renderer, surf);
    SDL_FreeSurface(surf);
    text = "Lvl ";
    text += std::to_string(level);
    surf = TTF_RenderText_Blended(Game::assets->GetFont("arial"),text.c_str(),color);
    levelLabel = SDL_CreateTextureFromSurface(Game::renderer, surf);
    SDL_FreeSurface(surf);
}

void Stats::KillEntity() {
    entity->destroy();
}

void Stats::update()
{
    if (health<=0)
    {
        if (!player)
        {
            KillEntity();
        }
    }
}

void Stats::draw()
{
    SDL_Rect destRect;
    SDL_Rect srcRect = {0,0,0,0};
    //Draw stats display
    //TODO: need to somehow get the current window size
    if(entity->type == "player")
    {
    int texWidth;
    int texHeight;
    int windowWidth;
    int windowHeight;
    int sep,h,w;
    w = 322*Game::windowSize_x/1600.0;
    h = w*150.0/322.0;
    sep = w*10.0/322.0;
    //Game::getWindowSize(&windowWidth,&windowHeight);
    destRect.x = sep;
    destRect.y = sep;
    destRect.w = w;
    destRect.h = h;
    SDL_SetRenderDrawColor(Game::renderer,255,255,255,150);
    SDL_RenderFillRect(Game::renderer,&destRect);

    //Render labels
    destRect.x += sep;
    destRect.y += sep;
    destRect.h = w*31.0/322.0;
    destRect.w = destRect.h*18.0/5.0;
    SDL_RenderCopy(Game::renderer,healthLabel,NULL,&destRect);

    //Render health bar

    destRect.y = destRect.y+destRect.h;
    destRect.h = w*30.0/322.0;
    destRect.w = (w*162.0/322.0)*(static_cast<float>(health)/static_cast<float>(max_health));
    SDL_SetRenderDrawColor(Game::renderer,34,139,34,200);
    SDL_RenderFillRect(Game::renderer,&destRect);
    destRect.x = destRect.x+destRect.w;
    destRect.w = (w*162.0/322.0)*(1-(static_cast<float>(health)/static_cast<float>(max_health)));
    SDL_SetRenderDrawColor(Game::renderer,254,254,254,200);
    SDL_RenderFillRect(Game::renderer,&destRect);
    //

    destRect.x = sep+sep;
    destRect.y = destRect.y + destRect.h + w*8.0/322.0;
    destRect.h = w*31.0/322.0;
    destRect.w = destRect.h*9.0/5.0;
    SDL_RenderCopy(Game::renderer,expLabel,NULL,&destRect);

    //Render exp bar
    destRect.y = destRect.y + destRect.h;
    destRect.w = (w*162.0/322.0)*(static_cast<float>(experience)/static_cast<float>(level*100));
    SDL_SetRenderDrawColor(Game::renderer,100,100,30,200);
    SDL_RenderFillRect(Game::renderer,&destRect);
    destRect.x = destRect.x+destRect.w;
    destRect.w = (w*162.0/322.0)*(1-(static_cast<float>(experience)/static_cast<float>(level*100)));
    SDL_SetRenderDrawColor(Game::renderer,254,254,153,200);
    SDL_RenderFillRect(Game::renderer,&destRect);
    destRect.x = sep+sep;
    destRect.w = destRect.h*6.0/5.0;
    destRect.y += destRect.h/3.0;
    destRect.h = destRect.h*2/3.0;
    SDL_RenderCopy(Game::renderer,levelLabel,NULL,&destRect);


    //Render current weapon
    //TODO: what component keeps track of the current weapon
    destRect.w = w*65.0/322.0;
    destRect.h = destRect.w*2;
    destRect.x = sep+sep+w*162.0/322.0+sep;
    destRect.y = sep+sep;

    Sword* weapon1 = &(entity->getComponent<Sword>());
    SDL_QueryTexture(weapon1->texture,NULL,NULL,&texWidth,&texHeight);
    srcRect.h = weapon1->animation.height;
    srcRect.w = weapon1->animation.width;
    if(weapon1->is_equiped)
    {
        SDL_SetRenderDrawColor(Game::renderer,0,254,100,240);
        SDL_RenderFillRect(Game::renderer,&destRect);
    }
    SDL_RenderCopy(Game::renderer,weapon1->texture,&srcRect,&destRect);

    Range_Weapon* weapon2 = &(entity->getComponent<Range_Weapon>());
    destRect.x += destRect.w;
    srcRect.h = weapon2->animation.height;
    srcRect.w = weapon2->animation.width;
    if(weapon2->is_equiped)
    {
            SDL_SetRenderDrawColor(Game::renderer,0,254,100,240);
            SDL_RenderFillRect(Game::renderer,&destRect);
    }
    SDL_RenderCopy(Game::renderer,weapon2->texture,&srcRect,&destRect);

    }

    //
    else{
    TransformComponent* entityTransform = &(entity->getComponent<TransformComponent>());
    SDL_SetRenderDrawColor(Game::renderer,34,139,34,200);
    destRect.w = entityTransform->width*entityTransform->scale*(static_cast<float>(health)/static_cast<float>(max_health));
    destRect.h = destRect.w*(1.0/8.0);
    destRect.x = entityTransform->position.x-Game::camera.x;
    destRect.y = entityTransform->position.y-Game::camera.y-destRect.h;
    SDL_RenderFillRect(Game::renderer,&destRect);
    SDL_SetRenderDrawColor(Game::renderer,254,254,254,200);
    destRect.x = entityTransform->position.x-Game::camera.x+destRect.w;
    destRect.w = entityTransform->width*entityTransform->scale*(1-(static_cast<float>(health)/static_cast<float>(max_health)));
    destRect.h = destRect.w*(1.0/8.0);
    destRect.y = entityTransform->position.y-Game::camera.y-destRect.h;
    SDL_RenderFillRect(Game::renderer,&destRect);
    }
}

void Stats::GainExp(int exp) {
    // Handles level ups, level downs, and corresponding stat changes.
    if (player) { // Will ignore GainExp if not a player.
        experience += exp;
        bool level_change = false;
        while (experience > level*100) {
            experience -= level*100;
            level += 1;
            level_change = true;
        } while (experience < 0) {
            if (level == 1) {
                experience = 0;
            } else {
                level -= 1;
                experience += level*100;
                level_change = true;
            }
        }
        if (level_change) {
            int prev_max = max_health;
            max_health = 50 + 5 * level;
            damage_mult = 1 + 0.1 * level;
            if (prev_max < max_health) {
                health += max_health - prev_max;
            } else if (prev_max > max_health && health > max_health) {
                health = max_health;
            }
            std::string text;
            SDL_Colour color = {0,0,0,255};
            text = "Lvl ";
            text += std::to_string(level);
            SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont("arial"),text.c_str(),color);
            levelLabel = SDL_CreateTextureFromSurface(Game::renderer, surf);
            SDL_FreeSurface(surf);
        }
    }
    std::cout << level << std::endl;
    std::cout << damage_mult << std::endl;
}
