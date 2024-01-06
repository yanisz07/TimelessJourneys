#include "game.hpp"
#include "TextureManager.hpp"
#include "map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"
#ifdef __APPLE__
#include <CoreGraphics/CGDirectDisplay.h>
#endif
#ifdef _WIN32
#include <Windows.h>
#endif
#include <sstream>
#include <variant>
#include <filesystem>
#include "world.hpp"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;

//Camera
SDL_Rect Game::camera = {0,0,1600,1280};
int x_diff = 400; //Camera.x with respect to the position x of the player
int y_diff = 320; //Camera.y with respect to the position y of the player

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;


//Add characters
auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& player_health(manager.addEntity());
auto& enemy(manager.addEntity());
//test second enemy
auto& enemy2(manager.addEntity());
auto& enemy_health(manager.addEntity());
//End


std::filesystem::path projectDir = std::filesystem::current_path();

bool playerInvincible = false;
int t = 0;
Uint32 playerInvincibleStartTime = 0; // the player invincibility start time
Uint32 playerInvincibleDuration = 3000; // 3000 milliseconds

//Test collision with rotated objects
auto& TestCol(manager.addEntity());
//

Game::Game()
{
    isMenuOpen = true; // Menu status, starts with menu opened
    isFullscreen = false; // full screen statusm, Starts fullscreen mode
}

Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    //Initialize system section
    {
    int flags = 0;

    screen_width = width;
    screen_height = height;

    //3200 * 2560 is the size of the map
    camera.w = 3200 - screen_width;
    camera.h = 2560 - screen_height;
    x_diff = (width - 128)/2;
    y_diff = (height - 128)/2;

    isFullscreen = fullscreen;

    if (fullscreen)
    {
        flags=SDL_WINDOW_FULLSCREEN;

        #ifdef __APPLE__
        CGDirectDisplayID displayID = kCGDirectMainDisplay;
        screen_width = CGDisplayPixelsWide(displayID);
        screen_height = CGDisplayPixelsHigh(displayID);
        #endif

        #ifdef _WIN32
        screen_width = GetSystemMetrics(SM_CXSCREEN);
        screen_height = GetSystemMetrics(SM_CYSCREEN);
        #endif

        x_diff = (screen_width - 128)/2;
        y_diff = (screen_height - 128)/2;
        camera.w = 3200 - screen_width;
        camera.h = 2560 - screen_height;

    }

    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        std::cout << "Subsystems initialized!..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, screen_width, screen_height, flags);

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
    assets->AddTexture("projectile", "/assets/proj.png");

    //Load JSON data
    std::string path = "";
    std::string root2 = ROOT_DIR;
    path += root2;
    path += "/assets/World_1.json";
    assets->loadWorld(path);
    std::cout << "Character textures added" << std::endl;
    //End

    //Textures, map and fonts

    assets->AddTexture("enemy_projectile", "/assets/proj.png");
    assets->AddTexture("player_projectile", "/assets/proj.png");

    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map.map").string();
    std::string fontPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Arial.ttf").string();

    assets->AddFont("arial", fontPath.c_str(),16);

    map = new Map("terrain", 4, 32);

    //ecs implementation

    map->LoadMap(mapPath.c_str(), 25, 20);
    }

    //Create player and enemy
    {
    player.setType("player");
    player.addComponent<TransformComponent>(1400,1100,48,48,3,5);
    player.addComponent<SpriteComponent>(true, "player");
    player.getComponent<SpriteComponent>().setActions();
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addComponent<Stats>(true);
    //player.addComponent<WeaponComponent>(&manager);
    player.addComponent<WeaponComponent>(&manager);
    player.getComponent<WeaponComponent>().equip();

    player.addGroup(Game::groupPlayers);

    std::cout << "Player created" << std::endl;

    enemy.addComponent<TransformComponent>(1200,1000,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    enemy.addGroup(Game::groupEnemies);

    std::cout << "Enemy created" << std::endl;

    //create second enemy

    enemy2.addComponent<TransformComponent>(1300,1000,128,128,1);
    enemy2.addComponent<SpriteComponent>(true, "enemy");
    enemy2.getComponent<SpriteComponent>().setActions();
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<Stats>();
    enemy2.addGroup(Game::groupEnemies);
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

    //Test collision with rotated objects

    TestCol.addComponent<ColliderComponent>("terrain",1700,1300,200,100);
    TestCol.getComponent<ColliderComponent>().SetAngle(135);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& MapColliders(manager.getGroup(Game::groupMapColliders));
auto& PlayerProjectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& EnemyProjectiles(manager.getGroup(Game::groupEnemyProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& PlayerAttacks(manager.getGroup(Game::groupPlayerAttack));

void Game::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_m:
            Menu::toggleMenuState(isMenuOpen); // Existing menu toggle
            break;
        case SDLK_f:
            //3200 * 2560 is the size of the map
            if (isFullscreen) {
                SDL_SetWindowFullscreen(window, 0);// Set to windowed mode
                SDL_SetWindowSize(window,800,640); //Fix size
                //fix camera
                screen_width = 800;
                screen_height = 640;
                isFullscreen = false;
            } else
            {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); // Set to fullscreen mode
                isFullscreen = true;

                //fix camera
                #ifdef __APPLE__
                CGDirectDisplayID displayID = kCGDirectMainDisplay;
                screen_width = CGDisplayPixelsWide(displayID);
                screen_height = CGDisplayPixelsHigh(displayID);
                #endif

                #ifdef _WIN32
                screen_width = GetSystemMetrics(SM_CXSCREEN);
                screen_height = GetSystemMetrics(SM_CYSCREEN);
                #endif
            }
            //fix camera
            camera.w = 3200-screen_width;
            camera.h = 2560-screen_height;
            x_diff = (screen_width - 128)/2;
            y_diff = (screen_height - 128)/2;

            break;

        }
        break;


    case SDL_MOUSEMOTION:
        mousePosition.x = event.motion.x;
        mousePosition.y = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (isMenuOpen) {
            // Get the mouse coordinates and screen size
            int x, y ,screenWidth, screenHeight;
            SDL_GetMouseState(&x, &y);
            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
            // Menu button dimensions
            int buttonWidth = 150;
            int buttonHeight = 40;
            // Calculating location of buttons
            int centerX = (screenWidth - buttonWidth) / 2;
            int centerY = (screenHeight - 2 * buttonHeight - 20) / 2 + 100;

            //if click is within button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > centerY && y < centerY + buttonHeight) {
                isMenuOpen = false;
            }
        }
        break;
    default:
        break;
    }
}


void Game::update()
{
    if (!isMenuOpen)
    {
        Uint32 currentTime0 = SDL_GetTicks();
        // Get player/enemy info.
        SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
        Vector2D playerPos = player.getComponent<TransformComponent>().position;

        std::stringstream ssp; //hold variables and turn them into strings
        ssp << "Player position: " << playerPos;

        label.getComponent<UILabel>().SetLabelText(ssp.str(), "arial"); //change the text
        //End

        // Clear inactive entities and run update
        manager.refresh();
        manager.update();
        //End

        //Check and solve player collisions.
        for (auto& c : MapColliders)
        {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            if(Collision::CheckCollision(c->getComponent<ColliderComponent>(), player.getComponent<ColliderComponent>()))
            {
                std::cout << "Hit wall" << std::endl;
                player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
            }
        }
        //End

        //Test collision with rotated objects

        if (Collision::CheckCollision(TestCol.getComponent<ColliderComponent>() ,player.getComponent<ColliderComponent>()))
        {
            std::cout << "Hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }

        //

        for (auto& p : EnemyProjectiles)

        {
            if(Collision::CheckCollision(player.getComponent<ColliderComponent>(),p->getComponent<ColliderComponent>()))
            {
                std::cout << "Hit player!" << std::endl;
                Stats::Damage(p->getComponent<Stats>(),player.getComponent<Stats>());
                p->destroy();
            }
        }
        //End

        //Check damage done to enemies

        Uint32 currentTime = SDL_GetTicks();

        for (auto& e : enemies)
        {
            if(Collision::CheckCollision(player.getComponent<ColliderComponent>(),e->getComponent<ColliderComponent>()))
                {
                    if (!playerInvincible) {
                        std::cout << "Player Hit!" << std::endl;
                        std::cout << "Damage done" << std::endl;

                        player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
                        Stats::Damage(e->getComponent<Stats>(),player.getComponent<Stats>());

                        playerInvincible = true;
                        playerInvincibleStartTime = currentTime0;
                    }
                }
                for (auto& p : PlayerProjectiles)
                {
                    if(Collision::CheckCollision(p->getComponent<ColliderComponent>(),e->getComponent<ColliderComponent>()))
                    {
                        std::cout << "Projectile hit enemy" << std::endl;
                        Stats::Damage(player.getComponent<Stats>(),e->getComponent<Stats>());
                        e->getComponent<Stats>().set_hit(true);
                        e->getComponent<Stats>().set_type_hit(false);
                        e->getComponent<Stats>().set_hit_time(SDL_GetTicks());
                        e->getComponent<Stats>().set_hit_direction(p->getComponent<TransformComponent>().velocity);
                        p->destroy();
                    }
                }
                for (auto& a : PlayerAttacks)
                {
                    if(Collision::CheckCollision(a->getComponent<ColliderComponent>(),e->getComponent<ColliderComponent>()))
                    {
                        if (!e->getComponent<Stats>().is_hit())
                            {
                                std::cout << "Melee hit enemy" << std::endl;
                                Stats::Damage(player.getComponent<Stats>(),e->getComponent<Stats>());
                                e->getComponent<Stats>().set_hit(true);
                                e->getComponent<Stats>().set_type_hit(true);
                                e->getComponent<Stats>().set_hit_time(SDL_GetTicks());
                                Vector2D direction = Vector2D(a->getComponent<TransformComponent>().x_direction,a->getComponent<TransformComponent>().y_direction);
                                e->getComponent<Stats>().set_hit_direction(direction);
                            }
                    }
                }
                //Enemy knockback
                if(e->getComponent<Stats>().is_hit())
                {
                    currentTime = SDL_GetTicks();
                    Vector2D direction = e->getComponent<Stats>().direction_hit();
                    Uint32 delay = currentTime - e->getComponent<Stats>().time_hit();
                    bool type = e->getComponent<Stats>().type_hit();
                    if (delay <= 200)
                    {
                        if(delay == 0)
                        {
                            e->getComponent<SpriteComponent>().Play("Hurt", false, 1);
                        }
                        if (delay >= 100)
                        {
                            if (delay <= 140)
                            {
                                if(!type)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*10;
                                    e->getComponent<TransformComponent>().position.y += direction.y*10;
                                }
                                else
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*20;
                                    e->getComponent<TransformComponent>().position.y += direction.y*20;
                                }
                            }
                            else if (delay <= 180)
                            {
                                if(!type)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*5;
                                    e->getComponent<TransformComponent>().position.y += direction.y*5;
                                }
                                else
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*10;
                                    e->getComponent<TransformComponent>().position.y += direction.y*10;
                                }
                            }
                            else
                            {
                                if(!type)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*2;
                                    e->getComponent<TransformComponent>().position.y += direction.y*2;
                                }
                                else
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*5;
                                    e->getComponent<TransformComponent>().position.y += direction.y*5;
                                }
                            }
                        }
                    }
                    else
                    {
                        e->getComponent<Stats>().set_hit(false);
                    }
                }
        }
        //End

        Vector2D enemyPos = enemy.getComponent<TransformComponent>().position;

        //Projectiles shot from the enemy we can generalize this to all ennemies
        if (currentTime - lastProjectileTime >= 2000)  // 2000 milliseconds = 2 seconds
        {
            // Create a projectile every two seconds
            assets->CreateProjectile(Vector2D(enemyPos.x, enemyPos.y), Vector2D(1, 0), 200, 2, "enemy_projectile",false);
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

        camera.x = player.getComponent<TransformComponent>().position.x - x_diff;
        camera.y = player.getComponent<TransformComponent>().position.y - y_diff;

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

        //check invincibility duration and change status
        if (playerInvincible) {
            if (currentTime0 - playerInvincibleStartTime >= 0 && currentTime0 - playerInvincibleStartTime < 1000 && t==0)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                t=1;
            }
            else if (currentTime0 - playerInvincibleStartTime >= 1000 && currentTime0 - playerInvincibleStartTime < 2000 && t==1)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                t=2;
            }
            else if (currentTime0 - playerInvincibleStartTime >= 2000 && currentTime0 - playerInvincibleStartTime < 3000 && t==2)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                t=3;
            }
            else if (currentTime0 - playerInvincibleStartTime >= playerInvincibleDuration && t==3)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                playerInvincible = false;
                t=0;
            }
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    if (isMenuOpen) {
    Menu::renderMenu(renderer, isMenuOpen, mousePosition); // Render the menu if it's open
    }
    else{


    for (auto& t : tiles)
    {
        t->draw();
    }

    for (auto& c : MapColliders)
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

    /*for (auto& a : PlayerAttacks)
    {
        a->draw();
    }*/

    label.draw();
    enemy_health.draw();
    player_health.draw();

    TestCol.draw();

    SDL_RenderPresent(renderer);
    }
}
void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
