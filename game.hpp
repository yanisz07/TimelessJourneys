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
#include "game_over.h"
#include <filesystem>
#include "sdl_mixer.h"

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
    void toggleFullScreen();

    bool is_running() {return isRunning;}

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static bool isRunning;
    bool isFullscreen;
    bool isMusic;
    static bool DisplayMap;
    static SDL_Rect camera;
    int screen_width;
    int screen_height;

    static AssetManager* assets;

    enum groupLabels : std::size_t //we can have up to 32 groups
    {
        groupMap,
        groupPlayers,
        groupMapColliders,
        groupPlayerProjectiles,
        groupEnemyProjectiles,
        groupEnemies,
        groupPlayerAttack
    };

private:
    int cnt = 0;
    SDL_Window *window;
    Mix_Music *bgMusic;
    Uint32 lastProjectileTime;
    bool isGameStarted;
    bool isMenuOpen;
    bool isInGameMenuOpen;
    bool isSettingsOpen;
    bool isGameOverOpen;
    SDL_Point mousePosition;
    SDL_Rect retryButtonRect;
    SDL_Rect exitButtonRect;
};

#endif // GAME_H
