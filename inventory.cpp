#include "inventory.h"
#include "SDL_image.h"
#include <iostream>
#include "items.h"
#include "TextureManager.hpp"
using namespace std;



SDL_Texture* loadTexture(const std::string& filePath, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    if (!tempSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

// Constructor
Inventory::Inventory() : pickedUpItemIndex(-1) {
    isVisible = false;
    windowRect = {100, 100, 400, 300};
    gridRows = 4; // Example: 4 rows
    gridCols = 5; // Example: 5 columns
    itemSlot = {0, 0, 50, 50}; // Example: each slot is 50x50 pixels
}



// Destructor
Inventory::~Inventory() {
    // Clean up if needed
}

void Inventory::show() {
    isVisible = true;
}


void Inventory::hide() {
    isVisible = false;
}

void Inventory::toggle() {
    isVisible = !isVisible;
}

//Manage items

void addItem(const Item* item);
void removeItem(const Item* item);
Item* getItem(int index);


//Implement this
void Inventory::addItem(const Item& item) {
    item.push_back(item);
}

void Inventory::addNewItem(const Item* item,const string iconPath, SDL_Renderer* renderer) {
    SDL_Texture* iconTexture = TextureManager::LoadTexture(iconPath.c_str());
    if (iconTexture != nullptr) {
        Item newItem(iconTexture, item);
        addItem(newItem); //We have to implement this
    } else {
        cerr << "Failed to load texture for item: " << item << endl;
    }
}


void Inventory::removeItem(const Item& item) {
    auto it = std::find_if(items.begin(), items.end(),
                           [&item](const Item& i) { return i.name == item.name; });
    if (it != items.end()) {
        if (it->icon != nullptr) {
            SDL_DestroyTexture(it->icon);  // Free the texture
            it->icon = nullptr;
        }
        items.erase(it);
    }
}

Item* Inventory::getItem(int index) {
    if (index >= 0 && index < items.size()) {
        return &items[index];
    }
    return nullptr; // Return null if index is out of range
}


//In the useItem method, you would include the logic specific to each item's effect:
void Inventory::useItem(int index) {
    if (index >= 0 && index < items.size()) {
        // i have not done it yet

        // This might mean increasing the player's health or else..
        std::cout << "Used item: " << items[index].name << std::endl;

        // Example: If it's a consumable, remove it after use
        removeItem(items[index]);
    }
}
/* Example usage effect: restoring health
    if (items[index].name == "Health Potion") {
        player.restoreHealth(50); // Assuming 'player' is an instance of the Player class
        removeItem(items[index]); */

void Inventory::useSelectedItem() {
    if (selectedSlotIndex >= 0 && selectedSlotIndex < items.size()) {
        useItem(selectedSlotIndex);
        // Optionally, after using the item, unselect it or select the next one
        selectedSlotIndex = (selectedSlotIndex + 1) % items.size();
    }
}


void Inventory::pickUpItem(int index) {
    if (index >= 0 && index < items.size()) {
        // i didn't implement the logic for picking up an item yet
        std::cout << "Picked up item: " << items[index].name << std::endl;
        pickedUpItemIndex = index;
    }
}

void Inventory::clearInventory() {
    for (auto& item : items) {
        if (item.icon != nullptr) {
            SDL_DestroyTexture(item.icon);
            item.icon = nullptr;
        }
    }
    items.clear();
}

void Inventory::moveSelection(int offset) {
    int newIndex = selectedSlotIndex + offset;

    // Calculate current row and column
    int currentRow = selectedSlotIndex / gridCols;
    int currentCol = selectedSlotIndex % gridCols;

    // Calculate new row and column
    int newRow = (newIndex / gridCols) % gridRows;
    int newCol = (newIndex % gridCols + gridCols) % gridCols; // Adding gridCols before modulo for proper wrapping

    // Check if the new index is valid (within the bounds of the grid)
    if (newRow >= 0 && newRow < gridRows && newCol >= 0 && newCol < gridCols) {
        selectedSlotIndex = newRow * gridCols + newCol;
    }
}

void Inventory::handleEvents(SDL_Event& event) {
}

const int ITEM_ICON_WIDTH = 32;
const int ITEM_ICON_HEIGHT = 32;


void Inventory::render(SDL_Renderer* renderer) {
    if (!isVisible) return;


    SDL_Color pink = {0, 0, 0, 255}; // RGBA for black

    // Set the renderer draw color to pink
    SDL_SetRenderDrawColor(renderer, pink.r, pink.g, pink.b, pink.a);

    // Draw a rectangle for the inventory window
    SDL_RenderFillRect(renderer, &windowRect);

    // Calculate the size of each slot based on the inventory window size
    int slotWidth = windowRect.w / gridCols;
    int slotHeight = windowRect.h / gridRows;

    // Render item slots
    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {
            SDL_Rect slotRect = {
                windowRect.x + col * slotWidth,
                windowRect.y + row * slotHeight,
                slotWidth,
                slotHeight
            };

            // Set color for the item slot (e.g., grey)
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            SDL_RenderDrawRect(renderer, &slotRect); // Draw slot border

            // Render item icon if it exists
            int index = row * gridCols + col;
            if (index < items.size() && items[index].icon != nullptr) {
                // Calculate the destination rectangle for the item icon inside the slot
                SDL_Rect iconRect = {
                    slotRect.x + (slotWidth - ITEM_ICON_WIDTH) / 2,
                    slotRect.y + (slotHeight - ITEM_ICON_HEIGHT) / 2,
                    ITEM_ICON_WIDTH,
                    ITEM_ICON_HEIGHT
                };
                SDL_RenderCopy(renderer, items[index].icon, nullptr, &iconRect);
            }
        }
    }

    // Highlight the selected slot
    if (selectedSlotIndex >= 0) {
        SDL_Rect selectedSlotRect = {
            windowRect.x + (selectedSlotIndex % gridCols) * slotWidth,
            windowRect.y + (selectedSlotIndex / gridCols) * slotHeight,
            slotWidth,
            slotHeight
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for highlight
        SDL_RenderDrawRect(renderer, &selectedSlotRect); // Draw highlight border
    }

    // Reset the renderer draw color to white after drawing the inventory
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
