#include "sword.hpp""
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "Stats.hpp"
#include <math.h>
#include "../inventory.h"

Sword::Sword(Manager *man) : dmg_multiplier(1.0f)
{
    timer.start();
    timer.setTimeOut(reloadTime);
    std::cout << "Melee Attack" << std::endl;
    manager = man;
    std::string swoosh_effect_path = (projectDir / ".." / "TimelessJourneys" / "assets" / "soundAssets" /"sword_swoosh_effect1.mp3").string();
    swordSwooshSound = Mix_LoadWAV(swoosh_effect_path.c_str());
    if (!swordSwooshSound) {
        // Handle loading error
        std::cerr << "Failed to load sword_swoosh_effect.mp3: " << Mix_GetError() << std::endl;
    }
    attack = "sword";
    damage = 5;
    std::string spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "weaponAssets" / "BlueSword.png").string();
    texture = IMG_LoadTexture(Game::renderer,spritePath.c_str());
    srcR.w = animation.width;
    srcR.h = animation.height;
    destR.w = 27;
    destR.h = 54;
}

int Sword::equip()
{
    std::string type = entity->type;
    sprite = &(entity->getComponent<SpriteComponent>());

    std::map<std::string, Action> actions;
    actions = (entity->manager.getAssetManager()->world.Characters[type].Attacks[attack].Actions);
    Animation anim;
    for (auto it = actions.begin(); it != actions.end(); it++)
    {
        anim = Animation(it->second.y_0, 0, it->second.number_frames, 100, it->second.spriteName);
        sprite->animations[it->first] = anim;
    }
    std::cout << "weapon equiped" << std::endl;
    return 0;
}

void Sword::init()
{
    //texture = TextureManager::LoadTexture(spritePath.c_str()); need to link texture with equipped sword texture
    //dmg_multiplier = 1.0f;

    entityTransform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
}

void Sword::update()
{
    if (is_equiped)
    {
        if (Game::event.type == SDL_KEYDOWN && timer.timedOut())
        {
            switch(Game::event.key.keysym.scancode)
            {
            case
                SDL_SCANCODE_W:
                if(timer.timedOut())
                {
                    direction.x = entityTransform->x_direction;
                    direction.y = entityTransform->y_direction;
                    frontAttack();
                    is_attacking=true;
                }
                break;
            }
            timer.setTimeOut(reloadTime);
        }

        if (is_attacking)
        {
            if (timer.timedOut())
            {
                update_sword();
            }
        }
    }
}

int Sword::frontAttack()
{
    //attack implemented below only for player
    Vector2D entityPos = entityTransform->position;
    int scale = entityTransform->scale;
    auto& attackCol(manager->addEntity());
    if (direction.x == 0)
    {
        if (direction.y == -1)
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+18*scale+6,entityPos.y+13*scale-38,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(0,-1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+18*scale+6,entityPos.y+13*scale-38,27,54,100);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
        else
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+18*scale+6,entityPos.y+(13+19)*scale,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(0,1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+18*scale+6,entityPos.y+(13+19)*scale,27,54,100);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
    }
    if (direction.y == 0)
    {
        if (direction.x == -1)
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+30,entityPos.y+13*scale+6,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(-1,0);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+30,entityPos.y+13*scale+6,27,54,100,-90);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
        else
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+(18+13)*scale,entityPos.y+13*scale+6,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(1,0);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+(18+13)*scale,entityPos.y+13*scale+6,27,54,100,90);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
    }
    if (direction.x == 1)
    {
        if (direction.y == -1)
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+(13+18)*scale-13,entityPos.y+16,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(1,-1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+(13+18)*scale-13,entityPos.y+16,27,54,100,45);
            attackCol.addGroup(Game::groupPlayerAttack);


        }
        if (direction.y == 1)
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+(13+18)*scale-13,entityPos.y+(13+19)*scale-16,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(1,1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+(13+18)*scale-13,entityPos.y+(13+19)*scale-16,27,54,100,135);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
    }
    if (direction.x == -1)
    {
        if (direction.y == -1)
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+13*scale-3,entityPos.y+6,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(-1,-1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+13*scale-3,entityPos.y+6,27,54,100,135);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
        if (direction.y == 1)
        {
            attackCol.addComponent<TransformComponent>(entityPos.x+13*scale-6,entityPos.y+(13+19)*scale-20,54,27);
            attackCol.getComponent<TransformComponent>().set_directions(-1,1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+13*scale-6,entityPos.y+(13+19)*scale-20,27,54,100,225);
            attackCol.addGroup(Game::groupPlayerAttack);
        }
    }
    Mix_PlayChannel(-1,swordSwooshSound, 0);
    return 0;
}

void Sword::draw()
{
    if (is_attacking)
    {
        if (!timer.timedOut())
        {
            entityTransform = &(entity->getComponent<TransformComponent>());
            srcR.y = 0;
            int frame;
            frame = static_cast<int>((timer.getTimeOutStart() / animation.speed) % animation.frames);
            animation.index = animation.width*frame;
            srcR.x = animation.index;
            Vector2D entityPos = entityTransform->position;
            int scale = entityTransform->scale;
            if (direction.x == 0)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+18*scale+6 - Game::camera.x;
                    destR.y = entityPos.y+13*scale-38 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_NONE);
                }
                else
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+18*scale+6 - Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,NULL,NULL,SDL_FLIP_VERTICAL);
                }
            }
            if (direction.y == 0)
            {
                if (direction.x == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+30 - Game::camera.x;
                    destR.y = entityPos.y+13*scale+6 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-90,NULL,SDL_FLIP_NONE);
                }
                else
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+(18+13)*scale - Game::camera.x;
                    destR.y = entityPos.y+13*scale+6 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,90,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == 1)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+(13+18)*scale-13-Game::camera.x;
                    destR.y = entityPos.y+16-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,45,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+(13+18)*scale-13-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-16-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,135,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == -1)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+13*scale-3-Game::camera.x;
                    destR.y = entityPos.y+6-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-45,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+13*scale-6-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-20-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,225,NULL,SDL_FLIP_NONE);
                }
            }
        }
    }
}

int Sword::DoDamage(Stats &entity1, Stats &entity2)
{
    int calculatedDamage = static_cast<int>(damage * dmg_multiplier);
    entity2.SubtractHealth(calculatedDamage);

    //entity2.SubtractHealth(damage*entity1.get_damage_mult());
    if(entity1.is_player())
    {
        if (entity2.get_health()<=0)
        {
            entity1.GainExp(entity2.get_experience_worth());
        }
    }
    //return 0;
    return calculatedDamage;
}

void Sword::update_sword()
{
    is_attacking = false;
}


void Sword::sword_equip(Inventory& inventory)
{
    if (!inventory.items.empty()) {
        for (const auto& item : inventory.items) {
            if (dynamic_cast<Melee*>(item.second) != nullptr) {



                dynamic_cast<Melee*>(item.second)->is_equipped=true;


                break;
            }
        }
    }
}
