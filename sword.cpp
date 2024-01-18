#include "sword.h"
#include "SDL_image.h"
#include "game.hpp"
#include <iostream>
#include "TextureManager.hpp"
#include <filesystem>
using namespace std;

//std::filesystem::path ProjectDir = std::filesystem::current_path();

// Function to create a Melee Sword item
Melee::Melee(SDL_Texture* icon, int damage, int durability)
    : InventoryItem(icon, items::Sword){
    this->damage = damage;
    this->durability = durability;

    std::string swordIconPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "sword.png").string();
    SDL_Texture* swordIcon = TextureManager::LoadTexture(swordIconPath.c_str());


    /* if (swordIcon) {
        int swordDamage = 20; // Adjust as needed
        int swordDurability = 100; // Adjust as needed

        //Melee swordItem(swordIcon, swordName, swordDamage, swordDurability);
        //return swordItem;
    } else {
        cerr << "Failed to load texture for sword: " << swordName << endl;
        // Return a default Sword with null texture in case of failure
        return Melee(nullptr, swordName, 0, 0);
    } */
}

