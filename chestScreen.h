#pragma once

#ifndef CHESTSCREEN_H
#define CHESTSCREEN_H

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "SDL.h"
#include "inventory.h"


\
class ChestScreen {
public:
    ChestScreen();
    ~ChestScreen();

    void show();
    void hide();
    void toggle();

    void init();

    void moveItem(const Item& item);
    void moveSelection(int offset);
    int getTotalCols(){return gridCols;}

    void render(SDL_Renderer* renderer);

    bool isCurrentlyVisible() const { return isVisible; }
    bool isCurrentlyInChest() const { return inChest; }

    bool selectedSlot_inventory;
    int selectedSlotIndex = 0;


private:
    bool inChest;
    bool isVisible;
    SDL_Rect windowRect_chest;
    //SDL_Rect windowRect_inventory;
    std::map<int,Item*> items;
    int gridRows;
    int gridCols;
    int currentRow;
    int currentCol;
    SDL_Rect itemSlot;

};



#endif // CHESTSCREEN_H
