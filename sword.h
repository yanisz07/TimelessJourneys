#ifndef SWORD_H
#define SWORD_H
#pragma once
#include <string>
#include <vector>
#include "SDL.h"
#include "inventoryscreen.h"

class Sword : public InventoryItem {
public:
    // Constructor for Sword
    Sword(SDL_Texture* icon, const std::string& name, int damage, int durability)
        : InventoryItem(icon, name), damage(damage), durability(durability) {
    }

private:
    int damage;
    int durability;
};

#endif // SWORD_H
