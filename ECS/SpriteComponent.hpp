#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Components.hpp"
#include "SDL.h"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include <map>
#include "../AssetManager.hpp"
#include "../world.hpp"
#include "../timer.hpp"

class WeaponComponent;

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    std::string type = "none";

    Timer timer;

    bool animated = false;

public:
    std::string currentAction;
    //int animIndex = 0; //update x index in the sprites sheet
    std::map<std::string , Animation> animations; //stores animations
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(std::string id)
    {
        setTex(id);
    }

    SpriteComponent( bool isAnimated, std::string type)
    {
        animated = isAnimated;
        this->type = type;
    }

    void setActions()
    {
        //type = entity->type;
        std::map<std::string, Action> actions;
        actions = (entity->manager.getAssetManager()->world.Characters[type].Actions);

        for (auto it = actions.begin(); it != actions.end(); it++)
        {
            //animations.emplace(it->first, Animation(it->second.y_0, 0, it->second.number_frames, 100, it->second.spriteName));
            animations[it->first] =  Animation(it->second.y_0, 0, it->second.number_frames, 100, it->second.spriteName);
        }

        actions = (entity->manager.getAssetManager()->world.Characters["player"].Attacks["sword"].Actions);
        Animation anim;
        for (auto it = actions.begin(); it != actions.end(); it++)
        {
            anim = Animation(128, 0, 8, 100, "Enemy_Idle_Sprite");
            //animations[it->first] = anim;
        }
        //animations["Attack_Down"] = Animation(48,0,4,100,"Player_Sprite_Attack_Down");

        if (type=="player")
        {
            Play("Idle_Down");
        }
        else
        {
            Play("Idle");
        }
    }

    void addAnimation(std::string animName,Animation animation)
    {
        animations[animName] = animation;
    }

    ~SpriteComponent()
    {}

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void init() override
    {

        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override
    {
        if(animated)
        {
            if(animations[currentAction].repeat==-1)
            {
                srcRect.x = srcRect.w * static_cast<int>((animations[currentAction].timer.getTimeStart() / animations[currentAction].speed) % animations[currentAction].frames); //update x index in the sprites sheet
                if(animations[currentAction].timer.getTimeStart() > animations[currentAction].frames*animations[currentAction].speed)
                {
                    animations[currentAction].timer.start();
                }
            }
            else
            {
                if (animations[currentAction].repeat!=0)
                {
                    srcRect.x = srcRect.w * static_cast<int>((animations[currentAction].timer.getTimeStart() / animations[currentAction].speed) % animations[currentAction].frames); //update x index in the sprites sheet
                    if(animations[currentAction].timer.getTimeStart() > animations[currentAction].frames*animations[currentAction].speed-50)
                    {
                        animations[currentAction].timer.start();
                        animations[currentAction].repeat-=1;
                    }
                }
                else
                {
                    if (type=="player")
                    {
                        Play("Idle_Down");
                    }
                    else
                    {
                        Play("Idle");
                    }
                }
            }
        }

        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override
    {
        //SDL_Rect rectangle{0,0,128,128};
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(const char* animName, bool flip = false, const int repeat = -1, int speed = 100)
    {
        if (flip)
        {
            spriteFlip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            spriteFlip = SDL_FLIP_NONE;
        }
        currentAction = animName;
        //animIndex = animations[animName].index;

        setTex(animations[animName].spriteName);
        srcRect.x = srcRect.y = 0;
        srcRect.h = animations[animName].wh;
        srcRect.w = animations[animName].wh;
        animations[currentAction].repeat = repeat;
        animations[currentAction].speed = speed;
        animations[currentAction].timer.start();
    }
};

#endif // SPRITECOMPONENT_H
