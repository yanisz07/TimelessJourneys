#ifndef SWORD_H
#define SWORD_H
#include <filesystem>
//#include "Components.hpp"
#include "../game.hpp"
#include "WeaponComponent.hpp"


class Sword : public WeaponComponent
{
public:

    Sword(Manager *man) : WeaponComponent(man)
    {
        melee = true;
        attack = "sword";
        damage = 5;
        std::string spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "sword2.png").string();
        texture = IMG_LoadTexture(Game::renderer,spritePath.c_str());
    }
};


#endif // SWORD_H