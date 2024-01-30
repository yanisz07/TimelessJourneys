#include <iostream>
#include "game.hpp"


Game *game = nullptr;

int main(int argc, char* args[]) {

    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("GameWindow",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600,1280,true);



    while(game->is_running())
    {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        //std::cout << frameTime << std::endl;
        if (frameDelay > frameTime)
        {
            //std::cout << frameDelay - frameTime << std::endl;
            SDL_Delay(frameDelay - frameTime);

        }
    }

    game->clean();

    return 0;

}
