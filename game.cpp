#include "game.h"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include <filesystem>
#include "world.hpp"


Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;

SDL_Rect Game::camera = {0,0,1600,1280};

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& player_health(manager.addEntity());
auto& enemy(manager.addEntity());

std::filesystem::path projectDir = std::filesystem::current_path();

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


    //Load game assets

    assets->AddTexture("terrain" , "/assets/terrain_ss.png");

    //Load JSON data
    std::string path = "";
    std::string root2 = ROOT_DIR;
    path += root2;
    path += "/assets/World_1.json";
    assets->loadWorld(path);
    std::cout << "Player textures added" << std::endl;
    //End

    assets->AddTexture("projectile", "/assets/proj.png");
    assets->AddTexture("enemy" , "/assets/enemy.png");

    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map.map").string();
    std::string fontPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Arial.ttf").string();

    assets->AddFont("arial", fontPath.c_str(),16);

    map = new Map("terrain", 3, 32);

    //ecs implementation

    map->LoadMap(mapPath.c_str(), 25, 20);

    player.addComponent<TransformComponent>(800,640,128,128,1);
    player.addComponent<SpriteComponent>( true, "player");
    player.getComponent<SpriteComponent>().setActions();
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addComponent<Stats>();
    player.addComponent<WeaponComponent>();
    player.addGroup(Game::groupPlayers);

    std::cout << "Player created" << std::endl;



    enemy.addComponent<TransformComponent>(600,600,32,32,4);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(Game::groupEnemies);

    SDL_Color white = {255,255,255,255};
    SDL_Color green = {0,255,0,255};
    label.addComponent<UILabel>(10,10, "Test String", "arial", white, true);

    //display player's health on top of his head
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    player_health.addComponent<UILabel>(playerPos.x, playerPos.y, "Test String2", "arial", green, false);

    lastProjectileTime = SDL_GetTicks();

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

    std::stringstream ssp; //hold variables and turn them into strings
    ssp << "Player position: " << playerPos;

    label.getComponent<UILabel>().SetLabelText(ssp.str(), "arial"); //change the text

    manager.refresh();
    manager.update();

    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if(Collision::AABB(cCol, playerCol))
        {
            std::cout << "Hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }
    }

    for (auto& p : projectiles)
    {
        if(Collision::AABB(playerCol,p->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Hit player!" << std::endl;
            player.getComponent<Stats>().SubtractHealth(2);
            p->destroy();
        }
    }

    for (auto& e : enemies)
    {
        if(Collision::AABB(playerCol,e->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Hit enemy" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    //projectiles shot from the enemy we can generalize this to all ennemies
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastProjectileTime >= 2000)  // 2000 milliseconds = 2 seconds
    {
        // Create a projectile every two seconds
        //assets->CreateProjectile(Vector2D(600, 600), Vector2D(1, 0), 200, 2, "projectile");
        lastProjectileTime = currentTime;  // Update the last projectile creation time
    }

    //update place of the health string
    playerPos = player.getComponent<TransformComponent>().position;
    player_health.getComponent<UILabel>().SetPositionText(playerPos.x, playerPos.y);

    //update text of health
    std::stringstream ssh;
    ssh << "Health: " << player.getComponent<Stats>().get_health();
    player_health.getComponent<UILabel>().SetLabelText(ssh.str(),"arial");

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

    for (auto& e : enemies)
    {
        e->draw();
    }

    for (auto& p : projectiles)
    {
        p->draw();
    }

    label.draw();
    player_health.draw();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
