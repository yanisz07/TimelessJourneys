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
#include "menu.hpp"
#include "rule.hpp"
#include "game_over.hpp"
#include <filesystem>
#include "sdl_mixer.h"
#include "items.hpp"
#include "chestScreen.hpp"
#include "timer.hpp"

extern std::filesystem::path projectDir;

#ifndef ROOT_DIR
#define ROOT_DIR ""
#endif

class ColliderComponent;

class AssetManager;

class Inventory;

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
    void loadSetUpJSON(std::string path);
    //static void getWindowSize(int*w, int*h);

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
    static Inventory* inventory;

    static int windowSize_x,windowSize_y;

    static ChestScreen* chestScreen1;
    static ChestScreen* chestScreen2;

    static AssetManager* assets;
    Mix_Chunk* clickButton;

    enum groupLabels : std::size_t //we can have up to 32 groups
    {
        groupMap,
        groupPlayers,
        groupMapColliders,
        groupPlayerProjectiles,
        groupEnemyProjectiles,
        groupEnemies,
        groupEnemiesAttack,
        groupPlayerAttack,
        groupChests
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
    bool isRuleOpen;
    SDL_Point mousePosition;
    SDL_Rect retryButtonRect;
    SDL_Rect exitButtonRect;
    Timer timeElapsed;
    SDL_Texture* timeLabel;
    std::string mapPath;
    std::string fontPath;
    std::string itemsPath;
    std::string worldPath;
    std::string musicPath;

};

#endif // GAME_H
