#include "inventory.h"
#include <iostream>
#include "SDL_image.h"
#include "TextureManager.hpp"
#include <fstream>

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
        // You will need to create an Item from the JSON data
        Item* item; // Placeholder, implement item creation from JSON
        addItem(name, id, *item);
    }
}

void Inventory::saveToJSON(const std::string& filePath) const {
    std::ofstream outFile(filePath);
    nlohmann::json j;

    for (const auto& [name, pair] : items) {
        j[name] = {{"id", pair.first}}; // Add other Item details as necessary
    }

    outFile << j.dump(4); // Write JSON to file with indentation
}


bool Inventory::get_visibility() {
    return isVisible;
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
