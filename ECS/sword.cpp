#include "sword.h""
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "Stats.hpp"
#include <math.h>

Sword::Sword(Manager *man)
{
    timer.start();
    timer.setTimeOut(reloadTime);
    std::cout << "Melee Attack" << std::endl;
    manager = man;
    std::string swoosh_effect_path = (projectDir / ".." / "TimelessJourneys" / "assets" / "sword_swoosh_effect1.mp3").string();
    swordSwooshSound = Mix_LoadWAV(swoosh_effect_path.c_str());
    if (!swordSwooshSound) {
        // Handle loading error
        std::cerr << "Failed to load sword_swoosh_effect.mp3: " << Mix_GetError() << std::endl;
    }
    attack = "sword";
    damage = 5;
    std::string spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "BlueSword.png").string();
    texture = IMG_LoadTexture(Game::renderer,spritePath.c_str());
    srcR.w = width;
    srcR.h = height;
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
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(0,-1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+18*scale+6,entityPos.y+13*scale-15,27,54,100);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
        else
        {
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(0,1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+18*scale+6,entityPos.y+(13+19)*scale,27,54,100);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
    }
    if (direction.y == 0)
    {
        if (direction.x == -1)
        {
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(-1,0);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x,entityPos.y+13*scale+15,54,27,100);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
        else
        {
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(1,0);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+(18+13)*scale,entityPos.y+13*scale+15,54,27,100);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
    }
    if (direction.x == 1)
    {
        if (direction.y == -1)
        {
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(1,-1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+(13+18)*scale-10,entityPos.y-10,27,54,100,45);
            attackCol.addGroup(Game::groupPlayerAttack);


        }
        if (direction.y == 1)
        {
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(1,1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x+(13+18)*scale-10,entityPos.y+(13+19)*scale-10,27,54,100,135);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
    }
    if (direction.x == -1)
    {
        if (direction.y == -1)
        {
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(-1,-1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x-10,entityPos.y-10,27,54,100,135);
            attackCol.addGroup(Game::groupPlayerAttack);

        }
        if (direction.y == 1)
        {
            std::cout << "Melee attack" << std::endl;
            attackCol.addComponent<TransformComponent>();
            attackCol.getComponent<TransformComponent>().set_directions(-1,1);
            attackCol.addComponent<ColliderComponent>("player_attack",entityPos.x-10,entityPos.y+(13+19)*scale-10,27,54,100,225);
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
            frame = static_cast<int>((timer.getTimeOutStart() / speed) % frames);
            index = width*frame;
            srcR.x = index;
            Vector2D entityPos = entityTransform->position;
            int scale = entityTransform->scale;
            if (direction.x == 0)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+18*scale+6 - Game::camera.x;
                    destR.y = entityPos.y+13*scale-15 - Game::camera.y;
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
                    Vector2D center = Vector2D(entityPos.x+27,entityPos.y+13*scale+29);
                    destR.x = center.x - 14 - Game::camera.x;
                    destR.y = center.y - 27 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-90,NULL,SDL_FLIP_NONE);
                }
                else
                {
                    setPriority(sprite->priority - 1);
                    Vector2D center = Vector2D(entityPos.x+(18+13)*scale+27,entityPos.y+13*scale+29);
                    destR.x = center.x - 14 - Game::camera.x;
                    destR.y = center.y - 27 - Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,90,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == 1)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x+(13+18)*scale-10-Game::camera.x;
                    destR.y = entityPos.y-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,45,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x+(13+18)*scale-10-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,135,NULL,SDL_FLIP_NONE);
                }
            }
            if (direction.x == -1)
            {
                if (direction.y == -1)
                {
                    setPriority(sprite->priority - 1);
                    destR.x = entityPos.x-10-Game::camera.x;
                    destR.y = entityPos.y-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,-45,NULL,SDL_FLIP_NONE);
                }
                if (direction.y == 1)
                {
                    setPriority(sprite->priority + 1);
                    destR.x = entityPos.x-10-Game::camera.x;
                    destR.y = entityPos.y+(13+19)*scale-10-Game::camera.y;
                    SDL_RenderCopyEx(Game::renderer,texture,&srcR,&destR,225,NULL,SDL_FLIP_NONE);
                }
            }
        }
    }
}

int Sword::DoDamage(Stats &entity1)
{
    entity1.SubtractHealth(1);
    return 0;
}


void Sword::update_sword()
{
    is_attacking = false;
}
