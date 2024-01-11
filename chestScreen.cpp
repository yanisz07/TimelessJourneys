#include "chestScreen.h"
#include "inventoryscreen.h"

#include <iostream>
using namespace std;




// Constructor
ChestScreen::ChestScreen(){
    isVisible = false;
    windowRect_chest = {200, 150, 400, 600};
    gridRows = 6; // 4 rows
    gridCols = 4; // 5 columns
    itemSlot = {0, 0, 100, 100}; // Each slot is 100x100 pixels


    windowRect_inventory = {800, 150, 400, 600};
    gridRows = 6; // 4 rows
    gridCols = 4; // 5 columns
    itemSlot = {0, 0, 100, 100}; // Each slot is 100x100 pixels
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

void ChestScreen::render(SDL_Renderer* renderer) {
    if (!isVisible) return;


    SDL_Color black = {0, 0, 0, 255}; // RGBA for black

    // Set the renderer draw color to black
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);

    // Draw a rectangle for the window
    SDL_RenderFillRect(renderer, &windowRect_chest);

    SDL_SetRenderDrawColor(renderer, 125, 212, 0, 255);

    SDL_RenderFillRect(renderer, &windowRect_inventory);

}



