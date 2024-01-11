#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

//#include "Components.hpp"
#include "SDL.h"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include <map>
#include "../AssetManager.hpp"
#include "../world.hpp"
#include "../timer.hpp"
#include "TransformComponent.hpp"

//forward decleration
class Armor;

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
    int frame;

    SpriteComponent() = default;
    SpriteComponent(std::string id);

    SpriteComponent( bool isAnimated, std::string type);

    void setActions();

    void addAnimation(std::string animName,Animation animation);

    ~SpriteComponent()
    {}

    void setTex(std::string id);

    void init() override;

    void update() override;

    void draw() override;

    void Play(const std::string animName, bool flip = false, const int repeat = -1, int speed = 100);

};

#endif // SPRITECOMPONENT_H