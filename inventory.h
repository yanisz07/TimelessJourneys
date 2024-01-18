#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <map>
#include "SDL.h"
#include "items.h"
#include "TextureManager.hpp"
#include "json.hpp" // Include JSON library header for handling JSON files

class TextureManager;

class Inventory {
public:
    Inventory();
    ~Inventory();

    void show();
    void hide();
    void toggle();

    void handleEvents(SDL_Event& event);
    void changeSelection(int change);
    void render(SDL_Renderer* renderer);

    // Update functions to work with the Item class and a map
    void addItem(const std::string& name, int id, const Item& item);
    void removeItem(const std::string& name);
    const Item* getItem(const std::string& name) const;
    void pickUpItem(int index);

    void clearInventory();
    void addNewItem(const std::string& name, int id, const Item& item);
    void useItem(int index);
    void useSelectedItem(const std::string& name);

    // Function to load and save inventory to a JSON file
    void loadFromJSON(const std::string& filePath);
    void saveToJSON(const std::string& filePath) const;

    bool get_visibility();
    int getGridCols() const { return gridCols; }
    void moveSelection(int offset);

private:
    bool isVisible;
    SDL_Rect windowRect; // UI dimensions for the inventory screen
    std::map<std::string, std::pair<int, Item>> items; // Map to store items by name and ID
    int gridRows;
    int gridCols;
};

#endif // INVENTORY_H
