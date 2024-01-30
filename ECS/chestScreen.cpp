#include "chestScreen.hpp"
#include "../items.hpp"
#include "../game.hpp"
#include <algorithm>
#include <iostream>

// Constructor
ChestScreen::ChestScreen(){
    isVisible = false;

    windowRect_chest = {200, 200, 500, 500};
    windowRect_inventory = {800, 200, 500, 500};

    gridCols = Game::inventory->getGridCols();
    gridRows = gridCols;

    totalCols = 2*gridCols; // (for both windows)

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

    for (auto i = items.begin(); i != items.end(); i++)
    {
        delete (i->second);
    }
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

void ChestScreen::moveItem() {
    int index = currentRow * 10 + currentCol;
    if (selectedSlot_inventory == true) {
        if (index < (Game::inventory->items).size() && (Game::inventory->items)[index] != nullptr) {
            if (!(Game::inventory->items)[index]->is_equipped) {
                if (!addItem((Game::inventory->items)[index])) {
                    std::cout << "Item " << (Game::inventory->items)[index]->name <<
                        " moved from inventory to chest" << std::endl;
                    (Game::inventory->items)[index] = nullptr;
                } else {
                    std::cout << "Cannot move item" << std::endl;
                }
            } else {
                std::cout << "Cannot move equipped item" << std::endl;
            }
        }
    } else if (selectedSlot_inventory == false) {
        if (index < items.size() && items[index] != nullptr) {
            if (!items[index]->is_equipped) {
                if (!(Game::inventory->addItem(items[index]))) {
                    std::cout << "Item " << items[index]->name <<
                        " moved from chest to inventory" << std::endl;
                    items[index] = nullptr;
                } else {
                    std::cout << "Cannot move item" << std::endl;
                }
            } else {
                std::cout << "Cannot move equipped item" << std::endl;
            }
        }
    }
}



void ChestScreen::moveSelection(int offset) {
    int newIndex = (selectedSlotIndex + offset) % (gridRows*totalCols);

    if (newIndex % (totalCols) >= gridCols) {
        selectedSlot_inventory = true;
        currentCol = (newIndex % totalCols) - gridCols;
    }
    else {
        selectedSlot_inventory = false;
        currentCol = newIndex % totalCols;
    }
    currentRow = newIndex / totalCols;

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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderFillRect(renderer, &windowRect_chest);


    SDL_SetRenderDrawColor(renderer, 125, 212, 0, 255);
    SDL_RenderFillRect(renderer, &windowRect_inventory);

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

            SDL_Rect slotRect_inventory = {
                windowRect_inventory.x + col * slotWidth,
                windowRect_inventory.y + row * slotHeight,
                slotWidth,
                slotHeight
            };

            // Set color for the item slot (e.g., grey)
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            SDL_RenderDrawRect(renderer, &slotRect_chest); // Draw slot border
            SDL_RenderDrawRect(renderer, &slotRect_inventory);

            // Render item icon if it exists
            int index = row * gridCols + col;
            if (index < items.size() && items[10*(index/gridCols)+(index%gridCols)] != nullptr) {
                // Calculate the destination rectangle for the item icon inside the slot
                SDL_Rect iconRect_chest = {
                    slotRect_chest.x + (slotWidth - ITEM_ICON_WIDTH) / 2,
                    slotRect_chest.y + (slotHeight - ITEM_ICON_HEIGHT) / 2,
                    ITEM_ICON_WIDTH,
                    ITEM_ICON_HEIGHT
                };
                SDL_RenderCopy(renderer, items[10*(row)+col]->icon, nullptr, &iconRect_chest);
            }
            if (index < (Game::inventory->items).size() && (Game::inventory->items)[10*(row)+col] != nullptr) {
                // Calculate the destination rectangle for the item icon inside the slot
                SDL_Rect iconRect_inventory = {
                    slotRect_inventory.x + (slotWidth - ITEM_ICON_WIDTH) / 2,
                    slotRect_inventory.y + (slotHeight - ITEM_ICON_HEIGHT) / 2,
                    ITEM_ICON_WIDTH,
                    ITEM_ICON_HEIGHT
                };
                SDL_RenderCopy(renderer, (Game::inventory->items)[10*(index/gridCols)+(index%gridCols)]->icon, nullptr, &iconRect_inventory);
            }

        }
    }

    // Highlight the selected slot
    if (selectedSlotIndex >= 0) {
        if (selectedSlot_inventory == true) {
            SDL_Rect selectedSlotRect = {
                windowRect_inventory.x + currentCol * slotWidth,
                windowRect_inventory.y + currentRow * slotHeight,
                slotWidth,
                slotHeight
            };
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for highlight
            SDL_RenderDrawRect(renderer, &selectedSlotRect); // Draw highlight border
        }

        else if (selectedSlot_inventory == false) {
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
}

int ChestScreen::addItem(Item* item) {
for(int i = 0;i<gridRows;i++)
{
    for(int j=0;j<gridCols;j++)
    {
                int key = 10*i+j;
                if(items[key]==nullptr)
                {
                items[key] = item;
                return 0;
                }
    }
}
std::cout << "Cannot add item, chest full" << std::endl;
return 1;
}

void ChestScreen::draw()
{
    render(Game::renderer);
}





