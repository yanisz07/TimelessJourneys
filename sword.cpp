#include "sword.h"
#include "SDL_image.h"
#include "game.hpp"
#include <iostream>
#include "TextureManager.hpp"
#include <filesystem>
using namespace std;

std::filesystem::path ProjectDir = std::filesystem::current_path();

// Function to create a Sword item
/* Sword createSword(SDL_Renderer* renderer) {
    std::string swordName = "Excalibur";
    std::string swordIconPath = (ProjectDir / ".." / "TimelessJourneys" / "assets" / "sword.png").string();
    SDL_Texture* swordIcon = TextureManager::LoadTexture(swordIconPath.c_str());

    if (swordIcon) {
        int swordDamage = 20; // Adjust as needed
        int swordDurability = 100; // Adjust as needed

        Sword swordItem(swordIcon, swordName, swordDamage, swordDurability);
        return swordItem;
    } else {
        cerr << "Failed to load texture for sword: " << swordName << endl;
        // Return a default Sword with null texture in case of failure
        return Sword(nullptr, swordName, 0, 0);
    }
}

*/
