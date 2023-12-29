#ifndef GAME_H
#define GAME_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "stdio.h"
#include "string"
#include "charconv"
#include "vector"
#include "menu.h"
#include <filesystem>

extern std::filesystem::path projectDir;

#ifndef ROOT_DIR
#define ROOT_DIR ""
#endif


class ColliderComponent;

class AssetManager;

class Game
{
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    int  loadWorld();
    void update();
    void render();
    void clean();
    void toggleMenu();

    bool is_running() {return isRunning;}

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static bool isRunning;
    bool isFullscreen;

    static SDL_Rect camera;
    int screen_width;
    int screen_height;

    static AssetManager* assets;

    enum groupLabels : std::size_t //we can have up to 32 groups
    {
        groupMap,
        groupPlayers,
        groupColliders,
        groupPlayerProjectiles,
        groupEnemyProjectiles,
        groupEnemies
    };

private:
    int cnt = 0;
    SDL_Window *window;
    Uint32 lastProjectileTime;
    bool isMenuOpen;
    SDL_Point mousePosition;
};

#endif // GAME_H
