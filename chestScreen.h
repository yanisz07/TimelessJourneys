#ifndef CHESTSCREEN_H
#define CHESTSCREEN_H

#include <string>
#include <vector>
#include "SDL.h"
#include "inventoryscreen.h"


\
class ChestScreen {
public:
    ChestScreen();
    ~ChestScreen();

    void show();
    void hide();
    void toggle();

    void moveItem(const InventoryItem& item);
    void moveSelection(int offset);

    void render(SDL_Renderer* renderer);

    bool isCurrentlyVisible() const { return isVisible; }
    bool isCurrentlyInChest() const { return inChest; }
    int getTotalCols() const { return totalCols; }

    bool selectedSlot_inventory;
    int selectedSlotIndex = 0;


private:
    bool inChest;
    bool isVisible;
    SDL_Rect windowRect_chest;
    SDL_Rect windowRect_inventory;
    std::vector<InventoryItem> items;
    int gridRows;
    int gridCols;
    int totalCols;
    int currentRow;
    int currentCol;
    SDL_Rect itemSlot;

};



#endif // CHESTSCREEN_H
