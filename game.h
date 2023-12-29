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
#include "Vector2D.h"

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
    void render();
    void clean();

    void update();
    void update_player_and_ennemies(Vector2D playerPos);
    void update_collisions(Vector2D playerPos,SDL_Rect playerCol);
    void update_damage_to_ennemies(Uint32 currentTime0,Vector2D playerPos,SDL_Rect playerCol);
    void update_knockback(Uint32 currentTime);
    void update_ennemy_projectiles(Uint32 currentTime);
    void update_health(Vector2D playerPos, Vector2D enemyPos);
    void update_camera();
    void update_invincibility(Uint32 currentTime0);

    bool is_running() {return isRunning;}

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static bool isRunning;
    static SDL_Rect camera;
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
};

#endif // GAME_H
