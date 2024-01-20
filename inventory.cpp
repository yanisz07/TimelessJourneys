#include "inventory.h"
#include <iostream>
#include "SDL_image.h"
#include "TextureManager.hpp"
#include <fstream>
#include "items.h"


const int ITEM_ICON_WIDTH = 32;
const int ITEM_ICON_HEIGHT = 32;

// Constructor
Inventory::Inventory() : isVisible(false), windowRect{100, 100, 400, 300} {
    // Initialize any other members if necessary
}

// Destructor
Inventory::~Inventory() {
    // Perform necessary cleanup
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


void Inventory::render(SDL_Renderer* renderer) {
    if (!this->isVisible) return;

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
        if (index < items.size() && items.find(std::to_string(index)) != items.end() && items.at(std::to_string(index)).second.icon != nullptr) {
            // Calculate the destination rectangle for the item icon inside the slot
            SDL_Rect iconRect = {
                slotRect.x + (slotWidth - ITEM_ICON_WIDTH) / 2,
                slotRect.y + (slotHeight - ITEM_ICON_HEIGHT) / 2,
                ITEM_ICON_WIDTH,
                ITEM_ICON_HEIGHT
            };
            SDL_RenderCopy(renderer, items.at(std::to_string(index)).second.icon, nullptr, &iconRect);
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




void Inventory::addItem(const std::string& name, int id, const Item& item) {
    items.insert({name, {id, item}});
}

void Inventory::removeItem(const std::string& name) {
    items.erase(name);
}


const Item* Inventory::getItem(const std::string& name) const {
    auto it = items.find(name);
    if (it != items.end()) {
        return &(it->second.second);

    }
}


void Inventory::useItem(int index) {
    std::string indexKey = std::to_string(index);

    auto it = items.find(indexKey);
    if (it != items.end()) {
        std::cout << "Used item: " << it->second.second.name << std::endl;
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

    auto it = items.find(indexKey);
    if (it != items.end()) {
        std::cout << "Picked up item: " << it->second.second.name << std::endl;
        pickedUpItemIndex = index;
    } else {
        std::cout << "Item with index " << index << " not found." << std::endl;
    }
}

void Inventory::clearInventory() {
    items.clear();
}

void Inventory::addNewItem(const std::string& name, int id, const Item& item) {
    addItem(name, id, item);
}


void Inventory::loadFromJSON(const std::string& filePath) {
    std::ifstream inFile(filePath);
    nlohmann::json j;
    inFile >> j;

    // Clear current inventory
    clearInventory();

    // Load items from JSON
    for (auto& element : j.items()) {
        std::string name = element.key();
        int id = element.value()["id"];

        // Create an Item from the JSON data
        Item item = Item(
            false,  // Example: Set is_equipped to false, adjust as needed
            element.value()["location"],
            element.value()["spritePath"],
            element.value()["name"]
            );

        // Load icon from spritePath using TextureManager or other method
        item.icon = TextureManager::LoadTexture(item.spritePath.c_str()); // Adjust based on your actual loading method

        // Add the item to the inventory
        addItem(name, id, item);
    }
}


void Inventory::saveToJSON(const std::string& filePath) const {
    std::ofstream outFile(filePath);
    nlohmann::json j;

    for (const auto& [name, pair] : items) {
        const Item& item = pair.second; // Extract the item from the pair

        // Add all item details to the JSON
        j[name] = {
            {"id", item.is_equipped},
            {"location", item.location},
            {"spritePath", item.spritePath},
            {"name", item.name}
            // Add other Item details as necessary
        };
    }

    outFile << j.dump(4); // Write JSON to file with indentation
}

