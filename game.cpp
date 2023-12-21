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
auto& enemy_health(manager.addEntity());

std::filesystem::path projectDir = std::filesystem::current_path();

//Test for knockback on enemys
std::vector<Entity*> enemies_hit;
std::vector<Vector2D> projectiles_hit_enemies_directions;
std::vector<Uint32> hit_time;
//

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    //Initialize system section
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
    }



    //Load game assets
    {
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

    //Textures, map and fonts



    assets->AddTexture("enemy_projectile", "/assets/proj.png");
    assets->AddTexture("player_projectile", "/assets/proj.png");

    assets->AddTexture("green_blob_hurt", "/assets/Green_Slime/Hurt.png");
    assets->AddTexture("blue_blob_hurt", "/assets/Blue_Slime/Hurt.png");

    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map.map").string();
    std::string fontPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Arial.ttf").string();

    assets->AddFont("arial", fontPath.c_str(),16);

    map = new Map("terrain", 3, 32);

    //ecs implementation

    map->LoadMap(mapPath.c_str(), 25, 20);
    }

    //Create player and enemy
    {
    player.addComponent<TransformComponent>(800,640,128,128,1);
    player.addComponent<SpriteComponent>(true, "player");
    player.getComponent<SpriteComponent>().setActions();
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addComponent<Stats>();
    player.addComponent<WeaponComponent>();
    player.getComponent<WeaponComponent>().getTransformComponent();
    player.addGroup(Game::groupPlayers);

    std::cout << "Player created" << std::endl;

    enemy.addComponent<TransformComponent>(600,600,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    enemy.addGroup(Game::groupEnemies);

    std::cout << "Enemy created" << std::endl;
    }

    //Create labels
    {
    SDL_Color white = {255,255,255,255};
    SDL_Color green = {0,255,0,255};
    SDL_Color red = {255,0,0,255};
    label.addComponent<UILabel>(10,10, "Test String", "arial", white, true);

    //display player's health on top of his head
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    player_health.addComponent<UILabel>(playerPos.x, playerPos.y, "Test String2", "arial", green, false);

    //display enemy's health on top of his head
    Vector2D enemyPos = enemy.getComponent<TransformComponent>().position;

    enemy_health.addComponent<UILabel>(enemyPos.x, enemyPos.y, "Test String3", "arial", red, false);


    lastProjectileTime = SDL_GetTicks();

}
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& PlayerProjectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& EnemyProjectiles(manager.getGroup(Game::groupEnemyProjectiles));
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
    // Get player/enemy info.
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    Vector2D enemyPos = enemy.getComponent<TransformComponent>().position;

    std::stringstream ssp; //hold variables and turn them into strings
    ssp << "Player position: " << playerPos;

    label.getComponent<UILabel>().SetLabelText(ssp.str(), "arial"); //change the text
    //End

    // Clear inactive entities and run update
    manager.refresh();
    manager.update();
    //End

    //Check and solve player collisions.
    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if(Collision::AABB(cCol, playerCol))
        {
            std::cout << "Hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }
    }
    //End


    for (auto& p : EnemyProjectiles)

    {
        if(Collision::AABB(playerCol,p->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Hit player!" << std::endl;
            Stats::Damage(p->getComponent<Stats>(),player.getComponent<Stats>());
            p->destroy();
        }
    }
    //End

    //Check damage done to enemies by proximity
    for (auto& e : enemies)
    {
        SDL_Rect enemyCol = e->getComponent<ColliderComponent>().collider;
        if(Collision::AABB(playerCol,enemyCol))
        {
            std::cout << "Hit enemy" << std::endl;
            std::cout << "Damage done" << std::endl;

            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
            Stats::Damage(player.getComponent<Stats>(),enemy.getComponent<Stats>());
        }

        for (auto& p : PlayerProjectiles)
        {
            if(Collision::AABB(p->getComponent<ColliderComponent>().collider,enemyCol))
            {
                std::cout << "Projectile hit enemy" << std::endl;
                Stats::Damage(player.getComponent<Stats>(),enemy.getComponent<Stats>());
                //Test for enemy knockback
                enemies_hit.push_back(e);
                hit_time.push_back(SDL_GetTicks());
                projectiles_hit_enemies_directions.push_back(p->getComponent<TransformComponent>().velocity);
                p->destroy();
            }

        }
    }
    //

    Uint32 currentTime = SDL_GetTicks();

    //Test enemy knockback
    for (std::size_t i = 0; i < enemies_hit.size(); ++i)
    {
        Entity* enemy = enemies_hit[i];
        Vector2D direction = projectiles_hit_enemies_directions[i];
        currentTime = SDL_GetTicks();
        Uint32 delay = currentTime - hit_time[i];
        if (delay <= 500)
        {
            if (delay <= 250)
            {
                if (delay <= 0.1)
                {
                    enemy->getComponent<TransformComponent>().position.x += direction.x*10;
                    enemy->getComponent<TransformComponent>().position.y += direction.y*10;
                }
                else if (delay <= 0.3)
                {
                    enemy->getComponent<TransformComponent>().position.x += direction.x*5;
                    enemy->getComponent<TransformComponent>().position.y += direction.y*5;
                }
                else
                {
                    enemy->getComponent<TransformComponent>().position.x += direction.x*1;
                    enemy->getComponent<TransformComponent>().position.y += direction.y*1;
                }
            }
            enemy->getComponent<SpriteComponent>().Play("Hurt");
        }
        else
        {
            enemy->getComponent<TransformComponent>().velocity.x = 0;
            enemies_hit.erase(enemies_hit.begin() + i);
            hit_time.erase(hit_time.begin() + i);
            projectiles_hit_enemies_directions.erase(projectiles_hit_enemies_directions.begin() + i);
            enemy->getComponent<SpriteComponent>().Play("Idle");
        }
    }
    //End

    //Projectiles shot from the enemy we can generalize this to all ennemies
    if (currentTime - lastProjectileTime >= 2000)  // 2000 milliseconds = 2 seconds
    {
        // Create a projectile every two seconds
        assets->CreateProjectile(Vector2D(600, 600), Vector2D(1, 0), 200, 2, "enemy_projectile",false);
        lastProjectileTime = currentTime;  // Update the last projectile creation time
    }
    //End

    //update place of the player_health string
    playerPos = player.getComponent<TransformComponent>().position;
    player_health.getComponent<UILabel>().SetPositionText(playerPos.x, playerPos.y);

    //update text of health
    std::stringstream ssh;
    ssh << "Health: " << player.getComponent<Stats>().get_health();
    player_health.getComponent<UILabel>().SetLabelText(ssh.str(),"arial");

    //update position of the enemy_player string
    enemyPos = enemy.getComponent<TransformComponent>().position;
    enemy_health.getComponent<UILabel>().SetPositionText(enemyPos.x, enemyPos.y);

    //update text of health
    std::stringstream sseh;
    sseh << "Health: " << enemy.getComponent<Stats>().get_health();
    enemy_health.getComponent<UILabel>().SetLabelText(sseh.str(),"arial");

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

    for (auto& p : PlayerProjectiles)
    {
        p->draw();
    }

    for (auto& p : EnemyProjectiles)
    {
        p->draw();
    }

    label.draw();
    enemy_health.draw();
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
