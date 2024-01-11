#ifndef CHESTSCREEN_H
#define CHESTSCREEN_H

#include <string>
#include <vector>
#include "SDL.h"


\
class ChestScreen {
public:
    ChestScreen();
    ~ChestScreen();

    void show();
    void hide();
    void toggle();

    void render(SDL_Renderer* renderer);



private:
    bool isVisible;
    SDL_Rect windowRect_chest;
    SDL_Rect windowRect_inventory;
    int gridRows;
    int gridCols;
    SDL_Rect itemSlot;

};



#endif // CHESTSCREEN_H
