#include "Stats.hpp"
#include <SDL.h>
#include "TransformComponent.hpp"
#include "../game.hpp"
#include "sword.h"


void Stats::SubtractHealth(int i)
{
    health -= i;
    if (player) {
        if (health > max_health) {
            health = max_health;
        } else if (health < 0) {
            GameOver();
        }
    } else if (health < 0) {
        KillEntity();
    }
}

void Stats::Damage(Stats& entity1, Stats& entity2)
{ // TODO To be deleted. Weapons should handle damage and use SubtractHealth and read damage_mult.
    entity2.SubtractHealth(entity1.get_damage_mult());
}

void Stats::GameOver() {
    // TODO
}

void Stats::KillEntity() {
    // TODO
}

void Stats::draw()
{
    SDL_Rect destRect;
    //Draw stats display
    //TODO: need to somehow get the current window size
    if(entity->type == "player")
    {
    int windowWidth;
    int windowHeight;
    int x,y,h,w;
    x = 10;
    y = 10;
    w = 300;
    h = 300*(5.0/8.0);
    //Game::getWindowSize(&windowWidth,&windowHeight);
    destRect.x = x;
    destRect.y = y;
    destRect.w = w;
    destRect.h = h;
    SDL_SetRenderDrawColor(Game::renderer,255,255,255,150);
    SDL_RenderFillRect(Game::renderer,&destRect);

    //Render health bar
    SDL_SetRenderDrawColor(Game::renderer,34,139,34,200);
    destRect.h = (1.0/6.0)*h;
    destRect.w = 150*(static_cast<float>(health)/static_cast<float>(max_health));
    destRect.x = 20;
    destRect.y = 10+(1.0/3.0)*h;
    SDL_RenderFillRect(Game::renderer,&destRect);
    SDL_SetRenderDrawColor(Game::renderer,254,254,254,200);
    destRect.x = destRect.x+destRect.w;
    destRect.w = 150*(1-(static_cast<float>(health)/static_cast<float>(max_health)));
    SDL_RenderFillRect(Game::renderer,&destRect);

    //Render exp bar
    SDL_SetRenderDrawColor(Game::renderer,100,100,30,200);
    destRect.h = (1.0/6.0)*h;
    destRect.w = 150*(static_cast<float>(experience)/static_cast<float>(1000));
    destRect.x = 20;
    destRect.y = 10+(5.0/6.0)*h;
    SDL_RenderFillRect(Game::renderer,&destRect);
    SDL_SetRenderDrawColor(Game::renderer,254,254,153,200);
    destRect.x = destRect.x+destRect.w;
    destRect.w = 150*(1-(static_cast<float>(experience)/static_cast<float>(1000)));
    SDL_RenderFillRect(Game::renderer,&destRect);

    //Render current weapon
    //TODO: what component keeps track of the current weapon
    destRect.h = h-20;
    destRect.w = h;
    destRect.x = 150;
    destRect.y = 20;
    WeaponComponent* weapon = &(entity->getComponent<Sword>());
    SDL_RenderCopy(Game::renderer,weapon->texture,NULL,&destRect);
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
        }
    }
}
