#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"
#include "../world.hpp"
#include "../timer.hpp"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    std::string type = "none";

    Timer timer;


    bool animated = false;
    int frames = 0;
    int speed = 100; //delay between frames in milliseconds

public:
    std::string currentAction;
    int animIndex = 0; //update y index in the sprites sheet
    std::map<std::string , Animation> animations; //stores animations

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(std::string id)
    {
        setTex(id);
        timer.start();
    }

    SpriteComponent( bool isAnimated, std::string type)
    {
        animated = isAnimated;
        this->type = type;
        timer.start();
    }

    void setActions()
    {
        std::map<std::string, Action> actions;
        actions = (entity->manager.getAssetManager()->world.Characters[type].Actions);

        for (auto it = actions.begin(); it != actions.end(); it++)
        {
            animations.emplace(it->first, Animation(it->second.y_0, it->second.number_frames, 100, it->second.spriteName));
        }
        Play("Idle");
    }

    ~SpriteComponent()
    {
    }

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
            srcRect.x = srcRect.w * static_cast<int>((timer.getTimeDelta() / speed) % frames); //update x index in the sprites sheet
            if(timer.getTimeDelta() > frames*speed)
            {
                timer.partial();
            }
            Play("Idle");
        }

        srcRect.y = animIndex; // * transform->height; //update y index in the sprites sheet

        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;

    }

    void draw() override
    {
        SDL_Rect rectangle{0,0,128,128};
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(const char* animName, const int repeat = 1)
    {
        if (timer.timedOut() && currentAction != animName || strcmp( animName, "Idle") != 0)
        {
            currentAction = animName;
            frames = animations[animName].frames;
            animIndex = animations[animName].index;
            speed = animations[animName].speed;
            setTex(animations[animName].spriteName);
            timer.partial();
            int w, h;
            SDL_QueryTexture(texture,NULL,NULL,&w,&h);
            srcRect.h = h - animIndex;
            srcRect.w = w/frames;
            timer.setTimeOut(speed*frames*repeat);
        }
    }
};

#endif // SPRITECOMPONENT_H
