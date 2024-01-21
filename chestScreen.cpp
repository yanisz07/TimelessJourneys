#include "chestScreen.h"
#include "inventory.h"
#include "items.h"
#include "game.hpp"
#include <algorithm>
#include <iostream>
using namespace std;




// Constructor
ChestScreen::ChestScreen(){
    isVisible = false;

    windowRect_chest = {200, 150, 400, 600};
    //windowRect_inventory = {800, 150, 400, 600};

    gridRows = 5; // 6 rows
    gridCols = 5; // 4 columns (in each window)
    itemSlot = {0, 0, 100, 100}; // Each slot is 100x100 pixels
}

void ChestScreen::init()
{
    for(int i = 0;i<gridRows;i++)
    {
        for(int j=0;j<gridCols;j++)
        {
            int key = 10*i+j;
            items[key] = nullptr;
        }
    }
}


// Destructor
ChestScreen::~ChestScreen() {
    // Clean up if needed
}

void ChestScreen::show() {
    isVisible = true;
}


void ChestScreen::hide() {
    isVisible = false;
}

void ChestScreen::toggle() {
    isVisible = !isVisible;
}

void moveItem(const Item& item) {

}


void ChestScreen::moveSelection(int offset) {
    int newIndex = (selectedSlotIndex + offset) % (gridRows*gridCols);

    currentCol = newIndex % gridCols;

    currentRow = newIndex / gridCols;

    selectedSlotIndex = newIndex;


    // Check if the new index is valid (within the bounds of the grid)
    //if (currentRow >= 0 && currentRow < gridRows && currentCol >= 0 && currentCol < gridCols) {
    //    selectedSlotIndex = newRow * gridCols + newCol;
    //}
}


const int ITEM_ICON_WIDTH = 32;
const int ITEM_ICON_HEIGHT = 32;


void ChestScreen::render(SDL_Renderer* renderer) {
    if (!isVisible) return;

    SDL_Rect inventoryWindow = Game::inventory->windowRect;
    Game::inventory->windowRect = {700, 150, 400, 600};
    Game::inventory->render(Game::renderer,false);
    Game::inventory->windowRect = inventoryWindow;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderFillRect(renderer, &windowRect_chest);


    SDL_SetRenderDrawColor(renderer, 125, 212, 0, 255);


    // Calculate the size of each slot based on the window size
    int slotWidth = windowRect_chest.w / gridCols;
    int slotHeight = windowRect_chest.h / gridRows;

    // Render item slots
    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {
            SDL_Rect slotRect_chest = {
                windowRect_chest.x + col * slotWidth,
                windowRect_chest.y + row * slotHeight,
                slotWidth,
                slotHeight
            };

            // Set color for the item slot (e.g., grey)
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            SDL_RenderDrawRect(renderer, &slotRect_chest); // Draw slot border

            // Render item icon if it exists
            int index = row * gridCols + col;
            if (index < items.size() && items[10*(index/gridCols)+(index%gridCols)]->icon!= nullptr) {
                // Calculate the destination rectangle for the item icon inside the slot
                SDL_Rect iconRect_chest = {
                    slotRect_chest.x + (slotWidth - ITEM_ICON_WIDTH) / 2,
                    slotRect_chest.y + (slotHeight - ITEM_ICON_HEIGHT) / 2,
                    ITEM_ICON_WIDTH,
                    ITEM_ICON_HEIGHT
                };
                SDL_RenderCopy(renderer, items[10*(index/gridCols)+(index%gridCols)]->icon, nullptr, &iconRect_chest);
            }
        }
    }

    // Highlight the selected slot
    if (selectedSlotIndex >= 0) {
                SDL_Rect selectedSlotRect = {
                    windowRect_chest.x + currentCol * slotWidth,
                    windowRect_chest.y + currentRow * slotHeight,
                    slotWidth,
                    slotHeight
                };
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for highlight
                SDL_RenderDrawRect(renderer, &selectedSlotRect); // Draw highlight border
            }

    // Reset the renderer draw color to white after drawing the inventory
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

}




