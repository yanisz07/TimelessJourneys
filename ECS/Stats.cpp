#include "Stats.hpp"
#include <SDL.h>
#include "TransformComponent.hpp"
#include "../game.hpp"


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
    TransformComponent* entityTransform = &(entity->getComponent<TransformComponent>());
    SDL_Rect destRect;
    SDL_SetRenderDrawColor(Game::renderer,34,139,34,1);
    destRect.w = entityTransform->width*entityTransform->scale*(static_cast<float>(health)/static_cast<float>(max_health));
    destRect.h = destRect.w*(1.0/8.0);
    destRect.x = entityTransform->position.x-Game::camera.x;
    destRect.y = entityTransform->position.y-Game::camera.y-destRect.h;
    SDL_RenderFillRect(Game::renderer,&destRect);
    SDL_SetRenderDrawColor(Game::renderer,254,254,254,1);
    destRect.x = entityTransform->position.x-Game::camera.x+destRect.w;
    destRect.w = entityTransform->width*entityTransform->scale*(1-(static_cast<float>(health)/static_cast<float>(max_health)));
    destRect.h = destRect.w*(1.0/8.0);
    destRect.y = entityTransform->position.y-Game::camera.y-destRect.h;
    SDL_RenderFillRect(Game::renderer,&destRect);
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
