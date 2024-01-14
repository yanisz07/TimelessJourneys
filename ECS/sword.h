#ifndef SWORD_H
#define SWORD_H
#include <filesystem>
//#include "Components.hpp"
#include "../game.hpp"
#include "TransformComponent.hpp"
#include "WeaponComponent.hpp"
#include "../TextureManager.hpp"
#include "../AssetManager.hpp"

//forward decleration
class TransformComponent;
class SpriteComponent;
//end

class Sword : public WeaponComponent
{
public:

    std::string spritePath;
    int frames = 4;
    int frame = 0;
    int index = 0;
    int speed = 100;
    int width = 276;
    int height = 539;
    SDL_Rect srcR, destR;

    Sword(Manager *man) : WeaponComponent(man)
    {
        attack = "sword";
        damage = 5;
        std::string spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "BlueSword.png").string();
        texture = IMG_LoadTexture(Game::renderer,spritePath.c_str());
        srcR.w = width;
        srcR.h = height;
        destR.w = 27;
        destR.h = 54;
    }

    void update_sword();

    void draw() override;

private:
    TransformComponent* entityTransform;
};


#endif // SWORD_H
