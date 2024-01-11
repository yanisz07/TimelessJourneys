#include <iostream>
#include "game.hpp"
#include "SDL_image.h"

Game *game = nullptr;

int main(int argc, char* args[]) {

    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("GameWindow",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600,1280,true);

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return -1;
    }


    while(game->is_running())
    {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clean();

    // Shut down SDL_image
    IMG_Quit();

    return 0;

}
