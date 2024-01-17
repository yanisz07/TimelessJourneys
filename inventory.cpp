#include "inventory.h"
#include <iostream>
#include "SDL_image.h"
#include "items.h"
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
    items[name] = {id, item};
}

void Inventory::removeItem(const std::string& name) {
    items.erase(name);
}

Item* Inventory::getItem(const std::string& name) const {
    auto it = items.find(name);
    if (it != items.end()) {
        return &(it->second.second);
    }
    return nullptr;
}

void Inventory::clearInventory() {
    items.clear();
}

void Inventory::addNewItem(const std::string& name, int id, const Item& item) {
    addItem(name, id, item);
}

void Inventory::useSelectedItem(const std::string& name) {
    // Placeholder for actual implementation
    // This function would interact with the game logic to use an item
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
        Item item; // Placeholder, implement item creation from JSON
        addItem(name, id, item);
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
