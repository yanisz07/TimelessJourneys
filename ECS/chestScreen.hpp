#pragma once

#ifndef CHESTSCREEN_H
#define CHESTSCREEN_H

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "SDL.h"
#include "../inventory.hpp"
#include "ECS.hpp"


\
class ChestScreen : public Component{
public:
    ChestScreen();
    ~ChestScreen();

    void show();
    void hide();
    void toggle();

    void init() override;
    void draw() override;

    void moveItem();
    void moveSelection(int offset);
    int getTotalCols(){return totalCols;}

    int addItem(Item* item);

    void render(SDL_Renderer* renderer);

    bool isCurrentlyVisible() const { return isVisible; }
    bool isCurrentlyInChest() const { return inChest; }

    bool selectedSlot_inventory;
    int selectedSlotIndex = 0;


private:
    bool inChest;
    bool isVisible;
    SDL_Rect windowRect_chest;
    SDL_Rect windowRect_inventory;
    std::map<int,Item*> items;
    int gridRows;
    int gridCols;
    int totalCols;

    int currentRow;
    int currentCol;
    SDL_Rect itemSlot;

};



#endif // CHESTSCREEN_H
