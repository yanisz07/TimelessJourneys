#include "game.h"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;

SDL_Rect Game::camera = {0,0,1600,1280};

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& enemy(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{

    int flags = 0;

    if (fullscreen)
    {
        flags=SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        std::cout << "Subsystems initialized!..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window)
        {
            std::cout << "Window created" << std::endl;
        }

        renderer = SDL_CreateRenderer(window,-1,0);

        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created" << std::endl;
        }

        isRunning = true;
    }

    else
    {
        isRunning = false;
    }

    if(TTF_Init() == -1)
    {
        std::cout << "Error : SDL_TTF" << std::endl;
    }

    assets->AddTexture("terrain" , "/assets/terrain_ss.png");
    assets->AddTexture("player" , "/assets/player_anims.png");
    assets->AddTexture("projectile", "/assets/proj.png");

    assets->AddFont("arial", "/Users/aminezghal/Desktop/SDL2TEST2/assets/Arial.ttf",16);

    map = new Map("terrain", 3, 32);

    //ecs implementation

    map->LoadMap("/Users/aminezghal/Desktop/SDL2TEST2/assets/map.map", 25, 20);

    player.addComponent<TransformComponent>(4);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(Game::groupPlayers);

    SDL_Color white = {255,255,255,255};
    label.addComponent<UILabel>(10,10, "Test String", "arial", white);

    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,0), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 620), Vector2D(2,0), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(400, 600), Vector2D(2,1), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,-1), 200, 2, "projectile");

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));

void Game::handleEvents()
{

    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    std::stringstream ss; //hold variables and turn them into strings
    ss << "Player position: " << playerPos;

    label.getComponent<UILabel>().SetLabelText(ss.str(), "arial"); //change the text

    manager.refresh();
    manager.update();

    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if(Collision::AABB(cCol, playerCol))
        {
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }
    }

    for (auto& p : projectiles)
    {
        if(Collision::AABB(playerCol,p->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Hit player!" << std::endl;
            p->destroy();
        }
    }

    camera.x = player.getComponent<TransformComponent>().position.x- 400;
    camera.y = player.getComponent<TransformComponent>().position.y - 320;

    if(camera.x <0)
    {
        camera.x = 0;
    }
    if(camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > camera.w)
    {
        camera.x = camera.w;
    }
    if(camera.y > camera.h)
    {
        camera.y = camera.h;
    }

    //get velocity and speed of the player to update the tiles
    /*Vector2D pVel = player.getComponent<TransformComponent>().velocity;
    int pSpeed = player.getComponent<TransformComponent>().speed;

    for (auto t : tiles)
    {
        //move tiles when player moving in x axis
        t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed);
        //move tiles when player moving in y axis
        t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed);
    }*/

}

void Game::render()
{
    SDL_RenderClear(renderer);
    //this is were we would add stuff to render
    for (auto& t : tiles)
    {
        t->draw();
    }

    for (auto& c : colliders)
    {
        c->draw();
    }

    for (auto& p : players)
    {
        p->draw();
    }

    for (auto& p : projectiles)
    {
        p->draw();
    }

    label.draw();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
