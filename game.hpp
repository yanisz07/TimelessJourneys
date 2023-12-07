#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "stdio.h"
#include "iostream"
#include "string"
#include "charconv"

#ifndef ROOT_DIR
#define ROOT_DIR ""
#endif

class Game
{
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool is_running() {return isRunning;}

    static SDL_Renderer *renderer;
    static SDL_Event event;

private:
    int cnt = 0;
    bool isRunning;
    SDL_Window *window;
};

#endif // GAME_H
