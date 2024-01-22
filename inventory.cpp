#include "inventory.h"
#include <iostream>
#include "SDL_image.h"
#include "TextureManager.hpp"
#include <fstream>
#include "items.h"


const int ITEM_ICON_WIDTH = 32;
const int ITEM_ICON_HEIGHT = 32;

// Constructor
Inventory::Inventory() : isVisible(false), windowRect{100, 400, 400, 400} {
    // Initialize any other members if necessary
}

// Destructor
Inventory::~Inventory() {
    // Perform necessary cleanup
}

void Inventory::init()
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

void Inventory::show() {
    isVisible = true;
}

void Inventory::hide() {
    isVisible = false;
}

void Inventory::toggle() {
    isVisible = !isVisible;
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


void Inventory::render(SDL_Renderer* renderer, bool forceRender) {
    if (!this->isVisible && forceRender) return;

    SDL_Color pink = {0, 0, 0, 255}; // RGBA for black

    // Set the renderer draw color to pink
    SDL_SetRenderDrawColor(renderer, pink.r, pink.g, pink.b, pink.a);

    // Draw a rectangle for the inventory window
    SDL_RenderFillRect(renderer, &this->windowRect);

    // Calculate the size of each slot based on the inventory window size
    int slotWidth = this->windowRect.w / this->gridCols;
    int slotHeight = this->windowRect.h / this->gridRows;

    // Render item slots
    for (int index = 0; index < items.size(); ++index) {
        int col = index % this->gridCols;
        int row = index / this->gridCols;

        SDL_Rect slotRect = {
            this->windowRect.x + col * slotWidth,
            this->windowRect.y + row * slotHeight,
            slotWidth,
            slotHeight
        };

        // Set color for the item slot (e.g., grey)
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderDrawRect(renderer, &slotRect); // Draw slot border

        // Render item icon if it exists
        if (index <items.size() && items[10*(index/gridCols)+(index%gridCols)] != nullptr) {
            // Calculate the destination rectangle for the item icon inside the slot
            SDL_Rect iconRect = {
                slotRect.x + (slotWidth - ITEM_ICON_WIDTH) / 2,
                slotRect.y + (slotHeight - ITEM_ICON_HEIGHT) / 2,
                ITEM_ICON_WIDTH,
                ITEM_ICON_HEIGHT
            };
            SDL_RenderCopy(renderer, items[10*(index/gridCols)+(index%gridCols)]->icon, nullptr, &iconRect);
        }
    }

    // Highlight the selected slot
    if (this->selectedSlotIndex >= 0 && this->selectedSlotIndex < items.size()) {
        int col = this->selectedSlotIndex % this->gridCols;
        int row = this->selectedSlotIndex / this->gridCols;

        SDL_Rect selectedSlotRect = {
            this->windowRect.x + col * slotWidth,
            this->windowRect.y + row * slotHeight,
            slotWidth,
            slotHeight
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for highlight
        SDL_RenderDrawRect(renderer, &selectedSlotRect); // Draw highlight border
    }

    // Reset the renderer draw color to white after drawing the inventory
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}




int Inventory::addItem(Item* item) {
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
    std::cout << "Cannot add item, inventory full" << std::endl;
    return 1;
}

void Inventory::removeItem(const int key) {
    items[key]=nullptr;
}


const Item* Inventory::getItem(const int key) {
    return items[key];
}


void Inventory::useItem(int index) {
    int currentRow = index / gridCols;
    int currentCol = index % gridCols;
    int key = 10*currentRow+currentCol;

    Item* it = items[key];
    if (it != nullptr) {
        std::cout << "Used item: " << it->name << std::endl;

    } else {
        std::cout << "Item with index " << index << " not found." << std::endl;
    }
}

/* Example usage effect: restoring health
    if (items[index].name == "Health Potion") {
        player.restoreHealth(50); // Assuming 'player' is an instance of the Player class
        removeItem(items[index]); */

void Inventory::useSelectedItem(const std::string& name) {

    if (selectedSlotIndex >= 0 && selectedSlotIndex < items.size()) {
        useItem(selectedSlotIndex);
        // Optionally, after using the item, unselect it or select the next one
        selectedSlotIndex = (selectedSlotIndex + 1) % items.size();
    }
}


void Inventory::pickUpItem(int index) {
    std::string indexKey = std::to_string(index);

    //auto it = items.find(indexKey);
    //if (it != items.end()) {
    //    std::cout << "Picked up item: " << it->second.second.name << std::endl;
    //    pickedUpItemIndex = index;
    //} else {
    //    std::cout << "Item with index " << index << " not found." << std::endl;
    //}
}

void Inventory::clearInventory() {
    init();
}

void Inventory::addNewItem(const std::string& name, int id, const Item& item) {
    //addItem(name, id, item);
}


void Inventory::loadFromJSON(const std::string& filePath) {
    std::ifstream inFile(filePath);
    nlohmann::json j = nlohmann::json::parse(inFile);

    // Clear current inventory
    clearInventory();

    // Load items from JSON
    for (int i = 0; i < size(j["items"]); ++i) {
        nlohmann::json itemData = j["items"][i];
        std::string name = itemData["name"].get<std::string>();
        std::string type = itemData["type"].get<std::string>();
        std::string location = itemData["location"].get<std::string>();
        std::string spritePath = itemData["spritePath"].get<std::string>();
        float dmg_multiplier = itemData["dmg_multiplier"].get<float>();

        if (type=="Melee") {

            Item* item = new Melee(false,location,spritePath,name,dmg_multiplier);
            addItem(item);

        }
        if (type=="RangedWeapon") { 
            int range = itemData["range"].get<int>();
            int speed_arrow = itemData["speed_arrow"].get<int>();
            int speed_shooting = itemData["speed_shooting"].get<int>();
            Item* item = new RangedWeapon(false,location,spritePath,name,dmg_multiplier,range,speed_arrow,speed_shooting);
            addItem(item);
        }
        if (type=="Armor") {
            int health_increase = itemData["health_increase"].get<int>();
            Item* item = new ArmorItem(false,location,spritePath,name,dmg_multiplier,health_increase);
            addItem(item);
        }
        std::cout << "Item " << name << "created and added" << std::endl;
    }

}


void Inventory::saveToJSON(const std::string& filePath) const {

}
