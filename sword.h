#ifndef SWORD_H
#define SWORD_H
#pragma once
#include <string>
#include <vector>
#include "SDL.h"
#include "inventoryscreen.h"
#include "items.h"


class Melee : public InventoryItem {
public:
    // Constructor for Sword
    Melee(SDL_Texture* icon, int damage, int durability);

private:
    int damage;
    int durability;
};

#endif // SWORD_H
