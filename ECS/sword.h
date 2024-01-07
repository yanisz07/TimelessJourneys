#ifndef SWORD_H
#define SWORD_H
#include <filesystem>
#include "Components.hpp"
#include "../game.hpp"


class Sword : public WeaponComponent
{
public:

    Sword(Manager *man) : WeaponComponent(man)
    {
        melee = true;
        attack = "sword";
        std::string spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "sword.png").string();
        texture = IMG_LoadTexture(Game::renderer,spritePath.c_str());
    }
};


#endif // SWORD_H
