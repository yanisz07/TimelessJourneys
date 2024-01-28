#pragma once

#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "SDL.h"
//#include "TextureManager.hpp"
#include "json.hpp" // Include JSON library header for handling JSON files

class Game;

//class TextureManager;
class Item;

class Inventory {
public:
    std::map<int, Item*> items; // Map to store items by name and ID
    SDL_Rect windowRect; // UI dimensions for the inventory screen
    Inventory();
    ~Inventory();
    void init();

    void show();
    void hide();
    void toggle();

    bool get_visibility(){return isVisible;}
    int getGridCols() const { return gridCols; }
    void moveSelection(int offset);

    void handleEvents(SDL_Event& event);
    void changeSelection(int change);
    void render(SDL_Renderer* renderer,bool forceRender = true);

    int selectedSlotIndex;
    int pickedUpItemIndex;

    void pickUpItem(int index);

    // Update functions to work with the Item class and a map
    int addItem(Item* item);
    void removeItem(const int key);
    const Item* getItem(const int key);

    void clearInventory();
    void addNewItem(const std::string& name, int id, const Item& item);
    void useSelectedItem();
    void useItem(int index);

    // Function to load and save inventory to a JSON file
    void loadFromJSON(const std::string& filePath);
    void saveToJSON(const std::string& filePath) const;

    Game* game;

    void setGame(Game* game );



private:
    bool isVisible;



    int gridCols = 5;
    int gridRows = gridCols;
};

#endif // INVENTORY_H
