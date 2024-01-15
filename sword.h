#ifndef SWORD_H
#define SWORD_H
#pragma once
#include <string>
#include <vector>
#include "SDL.h"
#include "inventoryscreen.h"
#include "items.h"

class Sword1 : public InventoryItem {
public:
    // Constructor for Sword
    Sword1(SDL_Texture* icon, int damage, int durability)
        : InventoryItem(icon, items::Sword){
        this->damage = damage;
        this->durability = durability;
    }

private:
    int damage;
    int durability;
};

#endif // SWORD_H
