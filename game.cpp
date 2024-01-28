#include "game.hpp"
#include "TextureManager.hpp"
#include "map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"
#include "setting.h"
#include "ingame_menu.h"
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
bool Game::DisplayMap = false;



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

struct MapPing {
    Vector2D position;
    Uint32 creationTime;
    Uint32 duration;
};

Game::Game()
{
    isGameStarted = false ; // tracks if player has started the game yet
    isMenuOpen = true; // Menu status, starts with menu opened
    isInGameMenuOpen = false; //  status for in game menu, starts closed
    isSettingsOpen = false; // Menu status, starts with menu opened
    isGameOverOpen = false;
    isFullscreen = false; // full screen statusm, Starts fullscreen mode
    isMusic = true; // music state, music on by default

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

    map = new Map("terrain", 4, 32, &manager);

    //ecs implementation

    map->LoadMap(mapPath.c_str(), 25, 20);
    }

    //Handle the music

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        isRunning = false;
    }
    std::string MusicPath = (projectDir / ".." / "TimelessJourneys" / "medieval.mp3").string();
    std::cout << "Trying to load music from: " << MusicPath << std::endl;
    bgMusic = Mix_LoadMUS(MusicPath.c_str());
    if (!bgMusic) {
        std::cerr << "Failed to load background music from " << MusicPath << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
        // Handle the error, maybe exit or provide a notification
    } else {
        if (Mix_PlayMusic(bgMusic, -1) == -1) {
            std::cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }

    //Create player and enemy
    {
    player.setType("player");
    player.addComponent<TransformComponent>(1400,1100,48,48,3,5);
    player.addComponent<SpriteComponent>(true, "player");
    player.getComponent<SpriteComponent>().setActions();
    player.addComponent<Armor>();
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");

    player.addComponent<Stats>(true);
    //player.addComponent<WeaponComponent>(&manager);
    player.addComponent<Sword>(&manager);
    player.getComponent<Sword>().equip();

    player.addGroup(Game::groupPlayers);

    std::cout << "Player created" << std::endl;

    enemy.addComponent<TransformComponent>(1200,1000,128,128,1);
    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<EnemyMovement>(250,100,800,60,&playerTransform); //To be changed later on
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
        case SDLK_ESCAPE:
            InGameMenu::toggleInGameMenuState(isInGameMenuOpen); // Existing menu toggle
            break;
        case SDLK_f:
            toggleFullScreen();
            break;

        }
        break;        

    case SDL_MOUSEMOTION:
        mousePosition.x = event.motion.x;
        mousePosition.y = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (DisplayMap && isFullscreen) { // Only allow ping creation when DisplayMap is true and isFullscreen
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Convert screen coordinates to map coordinates
            int mapWidth = 3200; // Full map width
            int mapHeight = 2560; // Full map height
            int mapDisplayWidth = 1200; // Displayed map width
            int mapDisplayHeight = 800; // Displayed map height
            int xStart = (screen_width - mapDisplayWidth) / 2;
            int yStart = (screen_height - mapDisplayHeight) / 2;

            float xRatio = static_cast<float>(mouseX - xStart) / mapDisplayWidth;
            float yRatio = static_cast<float>(mouseY - yStart) / mapDisplayHeight;

            mapPing.position.x = xRatio * mapWidth;
            mapPing.position.y = yRatio * mapHeight;
            mapPing.isActive = true; // Set the ping to active
        }

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
            int Start_centerY = (screenHeight - 2 * buttonHeight - 20) / 2 + 100;
            int Setting_centerY = ((screenHeight - 2 * buttonHeight - 20) / 2 + 100) + 60;
            int exitCenterY = Setting_centerY + buttonHeight + 20;

            //if click is within start button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Start_centerY && y < Start_centerY + buttonHeight) {
                isMenuOpen = false;
                isGameStarted = true;
            }
            //if click is within Setting button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Setting_centerY && y < Setting_centerY + buttonHeight) {
                isSettingsOpen = true;
                isMenuOpen = false;

            }
            // Check if click is within the Exit button boundary
            if (x > centerX && x < centerX + buttonWidth &&
                y > exitCenterY && y < exitCenterY + buttonHeight) {
                isRunning = false;
            }
        }
        else if (isInGameMenuOpen) {
            // Get the mouse coordinates and screen size
            int x, y ,screenWidth, screenHeight;
            SDL_GetMouseState(&x, &y);
            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
            // Menu button dimensions
            int buttonWidth = 150;
            int buttonHeight = 40;
            // Calculating location of buttons
            int centerX = (screenWidth - buttonWidth) / 2;
            int Start_centerY = (screenHeight - 2 * buttonHeight - 20) / 2 + 100;
            int Setting_centerY = ((screenHeight - 2 * buttonHeight - 20) / 2 + 100) + 60;
            int exitCenterY = Setting_centerY + buttonHeight + 20;

            //if click is within resume button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Start_centerY && y < Start_centerY + buttonHeight) {
                isInGameMenuOpen = false;
            }
            //if click is within Setting button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Setting_centerY && y < Setting_centerY + buttonHeight) {
                isSettingsOpen = true;
                isInGameMenuOpen = false;

            }
            // Check if click is within the Exit button boundary
            if (x > centerX && x < centerX + buttonWidth &&
                y > exitCenterY && y < exitCenterY + buttonHeight) {
                isRunning = false;
            }
        }
        else if (isSettingsOpen) {
            // Get the mouse coordinates and screen size
            int x, y ,screenWidth, screenHeight;
            SDL_GetMouseState(&x, &y);
            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
            // Menu button dimensions
            int buttonWidth = 250;
            int buttonHeight = 40;
            // Calculating location of buttons
            int centerX = (screenWidth - buttonWidth) / 2;
            int ScreenDim_centerY = (screenHeight - 2 * buttonHeight - 20) / 2 + 100;
            int Music_centerY = ((screenHeight - 2 * buttonHeight - 20) / 2 + 100) + 60;
            int Back_centerY = ((screenHeight - 2 * buttonHeight - 20) / 2 + 100) + 120;


            //if click is within back button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Back_centerY && y < Back_centerY + buttonHeight) {
                if (isGameStarted) {
                isSettingsOpen = false;
                isInGameMenuOpen = true;
                }
                else if (!isGameStarted){
                isSettingsOpen = false;
                isMenuOpen = true;

                }
            }

            //if click is within Music boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Music_centerY && y < Music_centerY + buttonHeight) {
                // Toggle music state
                if (isMusic) { // Music is currently playing
                    Mix_PauseMusic(); // Pause the music
                    isMusic = false; // Update the flag
                } else { // Music is currently paused
                    Mix_ResumeMusic(); // Resume the music
                    isMusic = true; // Update the flag
                }
            }
            //if click is within Screen Dimension button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > ScreenDim_centerY && y < ScreenDim_centerY + buttonHeight) {
                toggleFullScreen();

            }
        }


        else if (isGameOverOpen) {
            int x, y ,screenWidth, screenHeight;
            SDL_GetMouseState(&x, &y);
            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
            int buttonWidth = 150;
            int buttonHeight = 40;
            int centerX = (screenWidth - buttonWidth) / 2;
            int centerY = (screenHeight - 5 * buttonHeight - 20) / 2 + 100;
            SDL_Rect retryButtonRect = {centerX, centerY, buttonWidth, buttonHeight};
            SDL_Rect exitButtonRect = {centerX, centerY + buttonHeight + 20, buttonWidth, buttonHeight};


            if (x > retryButtonRect.x && x < retryButtonRect.x + retryButtonRect.w &&
                y > retryButtonRect.y && y < retryButtonRect.y + retryButtonRect.h) {
                // Reset game state to start again
                player.getComponent<Stats>().set_health(50);
                player.getComponent<TransformComponent>().position = Vector2D(1400, 1100);
                isGameOverOpen = false;
            }

            // Check if click is within exit button boundary
            if (x > exitButtonRect.x && x < exitButtonRect.x + exitButtonRect.w &&
                y > exitButtonRect.y && y < exitButtonRect.y + exitButtonRect.h) {
                isRunning = false; // Exit the game
            }
        }

        break;



    default:
        break;
    }
}


// function called when changing screen dimensions
void Game::toggleFullScreen() {
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

}


void Game::update()
{
    if (!isMenuOpen && !isGameOverOpen && !isInGameMenuOpen )
    {
        // Get player/enemy info.
        SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
        Vector2D playerPos = player.getComponent<TransformComponent>().position;

        SDL_Rect enemyCol = enemy.getComponent<ColliderComponent>().collider;

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

            if (enemy.getComponent<EnemyMovement>().collisionCooldown > 0) continue;

            SDL_Rect e_cCol = c->getComponent<ColliderComponent>().collider;
            if(Collision::AABB(e_cCol, enemyCol))
            {
                std::cout << "Enemy hit wall" << std::endl;
                enemy.getComponent<EnemyMovement>().onCollision(); // the enemy doesn't move
            }
        }
        //End

        //Test collision with rotated objects

        if (Collision::CheckCollision(TestCol.getComponent<ColliderComponent>() ,player.getComponent<ColliderComponent>()))
        {
            std::cout << "Player hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }


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
                    playerInvincibleStartTime = currentTime;
                }
            }
            for (auto& p : PlayerProjectiles)
            {
                if(Collision::CheckCollision(p->getComponent<ColliderComponent>(),e->getComponent<ColliderComponent>()))
                {
                    std::cout << "Projectile hit enemy" << std::endl;
                    //p->getComponent<ProjectileComponent>().doDamage(e->getComponent<Stats>());
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

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_l:
                    if (!DisplayMap) {
                        DisplayMap = true;

                    }
                    else {
                        DisplayMap = false;
                    }
                    break;
            default:
                    break;

            }
        }

        //check invincibility duration and change status
        if (playerInvincible)
        {
            if (currentTime - playerInvincibleStartTime >= 0 && currentTime - playerInvincibleStartTime < 1000 && t==0)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                t=1;
            }
            else if (currentTime - playerInvincibleStartTime >= 1000 && currentTime - playerInvincibleStartTime < 2000 && t==1)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                t=2;
            }
            else if (currentTime - playerInvincibleStartTime >= 2000 && currentTime - playerInvincibleStartTime < 3000 && t==2)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                t=3;
            }
            else if (currentTime - playerInvincibleStartTime >= playerInvincibleDuration && t==3)
            {
                //player.getComponent<SpriteComponent>().Play("Hurt",1);
                playerInvincible = false;
                t=0;
            }
        }
        int playerHealth = player.getComponent<Stats>().get_health();
        if (playerHealth <= 0)
        {
            isGameOverOpen = true;
        }
    }
}

void Game::render()
{
    // Clear the renderer with the current drawing color
    SDL_RenderClear(renderer);
    if (isGameOverOpen) {
        Game_Over::renderGameOver(renderer, isGameOverOpen, mousePosition);
    }
    else if (isMenuOpen) {
    Menu::renderMenu(renderer, isMenuOpen, mousePosition); // Render the menu if it's open
    }
    else if (isInGameMenuOpen) {
        InGameMenu::renderInGameMenu(renderer, isInGameMenuOpen, mousePosition); // Render the In game menu if it's open
    }
    else if (isSettingsOpen) {
        Setting::renderSetting(renderer, isSettingsOpen, mousePosition, isFullscreen, isMusic); // Render the setting menu if it's open
    }
    else{

    int screenWidth, screenHeight;
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
    SDL_Color textColor = {255, 255, 255}; // White color for the text


    if (!DisplayMap) {

            // Render all regular game objects when not in map view
            for (auto& t : tiles) { t->draw(); }
            for (auto& c : MapColliders) { c->draw(); }
            for (auto& p : players) { p->draw(); }
            for (auto& e : enemies) { e->draw(); }
            for (auto& pp : PlayerProjectiles) { pp->draw(); }
            for (auto& ep : EnemyProjectiles) { ep->draw(); }

            // Render the UI elements over the game objects
            label.draw();
            enemy_health.draw();
            player_health.draw();
            TestCol.draw();

    }
     else {

        if (isFullscreen) {
                RenderFullscreenMap( renderer, screenWidth, screenHeight, map);
                renderPlayerPosition(renderer);

                if (mapPing.isActive) {
                        // Render the ping on the map
                        renderMapPing(renderer); // Call a separate function to render the ping
                }
            }

        else {
                renderWindowedMap();

           }

            // Render the game name at the top of the map
            const int topPadding = 10; // Padding from the top edge
            SDL_Color textColor = {255, 255, 255}; // White color for the text
            SDL_Surface* surfaceGameName = TTF_RenderText_Solid(Game::assets->GetFont("arial"), "Timeless Journeys", textColor);
            if (surfaceGameName) {
            SDL_Texture* textureGameName = SDL_CreateTextureFromSurface(renderer, surfaceGameName);
            SDL_Rect gameNameRect = {
                (screenWidth - surfaceGameName->w) / 2, // Centered horizontally
                topPadding,
                surfaceGameName->w,
                surfaceGameName->h
            };
            SDL_RenderCopy(renderer, textureGameName, NULL, &gameNameRect);
            SDL_FreeSurface(surfaceGameName);
            SDL_DestroyTexture(textureGameName);
            }

        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray background color

    }

    SDL_RenderPresent(renderer);
    }

}

void Game::RenderFullscreenMap(SDL_Renderer* renderer, int screenWidth, int screenHeight, Map* map) {

    const int BorderSize = 10; // Size of the border around the map
    const int MapDisplayWidth = 1200;
    const int MapDisplayHeight = 800;

    int tileWidth = (MapDisplayWidth - 2 * BorderSize) / map->size_x;
    int tileHeight = (MapDisplayHeight - 2 * BorderSize) / map->size_y;
    int xStart = (screenWidth - MapDisplayWidth) / 2 + BorderSize;
    int yStart = (screenHeight - MapDisplayHeight) / 2 + BorderSize;

    // Draw the border
    SDL_Rect borderRect = {xStart - BorderSize, yStart - BorderSize, MapDisplayWidth, MapDisplayHeight};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for the border
    SDL_RenderFillRect(renderer, &borderRect);

    // Set the map's background color
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray background color

    // Initialize the destination rectangle for the map tiles
    SDL_Rect destRect = {xStart, yStart, tileWidth, tileHeight};

    // Render each tile of the map
    for (int y = 0; y < map->size_y; ++y) {
    for (int x = 0; x < map->size_x; ++x) {
        auto& tile = tiles[y * map->size_x + x];
        auto& sprite = tile->getComponent<TileComponent>().texture;
        auto& srcRect = tile->getComponent<TileComponent>().srcRect;

        SDL_RenderCopy(renderer, sprite, &srcRect, &destRect);
        destRect.x += tileWidth; // Move to the next tile position horizontally
    }
    destRect.x = xStart; // Reset to the start of the next row
    destRect.y += tileHeight; // Move to the next tile position vertically
    }

    RenderLegend(renderer); // Call the function to render the legend
}

void Game::RenderLegend(SDL_Renderer* renderer) {
    const int legendX = 10;
    const int legendY = 10;
    const int legendWidth = 220; // Slightly wider to accommodate larger text
    const int legendHeight = 130; // Adjusted height to fit additional item

    // Outer border color (dark gray/brownish tone)
    SDL_Color borderColor = {50, 40, 30, 255};
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_Rect borderRect = {legendX, legendY, legendWidth, legendHeight};
    SDL_RenderFillRect(renderer, &borderRect);

    // Inner background color (lighter tone for contrast)
    SDL_Color backgroundColor = {120, 110, 100, 255};
    const int padding = 4; // Padding for the inner background
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_Rect backgroundRect = {legendX + padding, legendY + padding, legendWidth - 2 * padding, legendHeight - 2 * padding};
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Set the text color for the legend
    SDL_Color textColor = {255, 255, 255, 255}; // White text for the legend

    // Define icon sizes
    const int iconSize = 20;
    const int innerPadding = 10; // Padding inside the legend

    // Render the legend text and icons
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Rect textRect;

    std::string texts[4] = {"Player", "Water", "Forest", "Ping"};
    SDL_Color colors[4] = {{255, 0, 0, 255}, {0, 0, 255, 255}, {0, 128, 0, 255}, {0, 0, 255, 255}};
    int offsetY = legendY + innerPadding;

    for (int i = 0; i < 4; ++i) {
    // Draw the colored box for the item
    SDL_Rect colorBox = {legendX + innerPadding, offsetY, iconSize, iconSize};
    SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
    SDL_RenderFillRect(renderer, &colorBox);

    // Render the associated text
    textSurface = TTF_RenderText_Blended(Game::assets->GetFont("arial"), texts[i].c_str(), textColor);
    if (textSurface) {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        textRect = {legendX + iconSize + 2 * innerPadding, offsetY, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    offsetY += iconSize + innerPadding; // Move down for the next item
    }

    // Reset the renderer color to white for default rendering
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}


void Game::renderWindowedMap() {
    for (auto& t : tiles) {
    auto& tileComponent = t->getComponent<TileComponent>();
    tileComponent.setTileScale2(1); // Assuming scale2 is the desired scale for map display
    t->draw();
    }

    // Additional rendering code for the windowed map can be added here if needed
}

void Game::renderPlayerPosition(SDL_Renderer* renderer) {
    // Player's position in the game world
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    // Render Player Position as Text
    std::stringstream ss;
    ss << "Player Position: X=" << playerPos.x << ", Y=" << playerPos.y;
    SDL_Color textColor = {255, 255, 255, 255}; // White color for the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(Game::assets->GetFont("arial"), ss.str().c_str(), textColor);

    if (textSurface) {
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {10, screen_height - 30, textSurface->w, textSurface->h}; // Position the text at the bottom left corner
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    }

    int mapWidth = 3200;
    int mapHeight = 2560;

    // Dimensions for the fullscreen map display
    const int BorderSize = 10;
    const int MapDisplayWidth = 1200;
    const int MapDisplayHeight = 800;
    int xStart = (screen_width - MapDisplayWidth) / 2 + BorderSize;
    int yStart = (screen_height - MapDisplayHeight) / 2 + BorderSize;

    //  the player's position on the map display
    float xRatio = static_cast<float>(playerPos.x) / mapWidth;
    float yRatio = static_cast<float>(playerPos.y) / mapHeight;
    int dotX = xStart + static_cast<int>(xRatio * (MapDisplayWidth - 2 * BorderSize));
    int dotY = yStart + static_cast<int>(yRatio * (MapDisplayHeight - 2 * BorderSize));

    // Draw a red dot at the player's position
    const int dotSize = 10; // Size of the dot
    SDL_Rect dotRect = {dotX - dotSize / 2, dotY - dotSize / 2, dotSize, dotSize};    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RGBA for red
    SDL_RenderFillRect(renderer, &dotRect);

    // Reset the renderer color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Game::renderMapPing(SDL_Renderer* renderer) {
    int dotSize = 10;
    SDL_Rect pingRect = {
        static_cast<int>(mapPing.position.x * screen_width / 3200) - dotSize / 2,
        static_cast<int>(mapPing.position.y * screen_height / 2560) - dotSize / 2,
        dotSize,
        dotSize
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // RGBA for blue
    SDL_RenderFillRect(renderer, &pingRect);

    // Reset the renderer color to whatever was the default before
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for default rendering

}
void Game::clean()
{
    if (bgMusic != nullptr) {
        Mix_FreeMusic(bgMusic);
        bgMusic = nullptr;
    }
    isGameOverOpen = false;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
