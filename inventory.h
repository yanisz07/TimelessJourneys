#ifndef INVENTORYSCREEN_H
#define INVENTORYSCREEN_H
#include <string>
#include <vector>
#include "SDL.h"
#include "items.h"
#include "TextureManager.hpp"

/*
class InventoryItem {
public:
    SDL_Texture* icon;
    Item* item;

    InventoryItem(SDL_Texture* icon, Item* item) : icon(icon), item(item) {}
    ~InventoryItem() {
        delete item;
        if (icon != nullptr) {
            SDL_DestroyTexture(icon);  // Free the texture
            icon = nullptr;
        }
    }
};
*/

class Inventory {
public:
    Inventory();
    ~Inventory();

    void show();
    void hide();
    void toggle();

    /*
    void addItem(const InventoryItem& item);
    void removeItem(const InventoryItem& item);
    InventoryItem* getItem(int index);
    */

    void addItem(const Item* item);
    void removeItem(const Item* item);
    Item* getItem(int index);

    void clearInventory();
    void addNewItem(const Item* item,const std::string iconPath, SDL_Renderer* renderer);
    void useSelectedItem();


    void handleEvents(SDL_Event& event);
    void changeSelection(int change);
    void render(SDL_Renderer* renderer);

    int selectedSlotIndex;
    void useItem(int index);
    int pickedUpItemIndex;
    void pickUpItem(int index);

    bool isCurrentlyVisible() const { return isVisible; }
    int getGridCols() const { return gridCols; }

    void moveSelection(int offset);

private:
    bool isVisible;
    SDL_Rect windowRect;
    //std::vector<InventoryItem> items;
    int gridRows;
    int gridCols;
    SDL_Rect itemSlot;

};

#endif // INVENTORYSCREEN_H
