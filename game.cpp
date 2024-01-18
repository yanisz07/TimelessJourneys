#include "game.hpp"
#include "TextureManager.hpp"
#include "map.hpp"
#include "ECS/Components.hpp"
#include "ECS/Stats.hpp"
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
InventoryScreen* Game::inventoryScreen = new InventoryScreen();
ChestScreen* Game::chestScreen = new ChestScreen();


bool Game::isRunning = false;
bool Game::DisplayMap = false;

//click Button sound


//Add characters
auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& player_health(manager.addEntity());
auto& enemy(manager.addEntity());
//test second enemy
auto& enemy2(manager.addEntity());
auto& enemy_health(manager.addEntity());
// Add chests
auto& chest(manager.addEntity());
//End

bool chest_open = false;

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

    //assets->AddTexture("chest", "/assets/chest_01.png");

    assets->AddTexture("arrow", "/assets/arrow.png");

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
    if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) {
        std::cerr << "Mix_Init failed: " << Mix_GetError() << std::endl;
    }
     Mix_AllocateChannels(2);
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
    player.addComponent<Sword>(&manager);
    player.getComponent<Sword>().equip();
    player.addComponent<Range_Weapon>(&manager);

    player.addGroup(Game::groupPlayers);
    timeElapsed = Timer();
    timeElapsed.start();

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


    //create first chest

    chest.addComponent<TransformComponent>(900,900,16,16,5);
    chest.addComponent<SpriteComponent>(true, "chest");
    chest.getComponent<SpriteComponent>().setActions();
    chest.addComponent<ColliderComponent>("chest");
    chest.addComponent<InteractComponent>();
    chest.addGroup(Game::groupChests);
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
    TestCol.getComponent<ColliderComponent>().SetAngle(227);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& MapColliders(manager.getGroup(Game::groupMapColliders));
auto& PlayerProjectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& EnemyProjectiles(manager.getGroup(Game::groupEnemyProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& PlayerAttacks(manager.getGroup(Game::groupPlayerAttack));
auto& chests(manager.getGroup(Game::groupChests));

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    std::string click_path = (projectDir / ".." / "TimelessJourneys" / "assets" / "click_button1.mp3").string();
    clickButton = Mix_LoadWAV(click_path.c_str());
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
        case SDLK_e:  // Check for 'E' key
            if (chest_open == false) {
            inventoryScreen->toggle();  // Toggle the inventory screen
            }
            break;
        case SDLK_RETURN:

            for (auto& ch : chests)
            {
            if (inventoryScreen->isCurrentlyVisible() == false) {
                InteractComponent& interact = ch->getComponent<InteractComponent>();

               if (chest_open == false) {
                if (interact.PlayerCloseTo(player.getComponent<TransformComponent>()))
                {
                    std::cout << "Opened chest" << std::endl;
                    chest.getComponent<SpriteComponent>().Play("Active");
                    chest_open = true;
                    chestScreen->toggle();

                }
               }
               else {
                chest_open = false;
                std::cout << "Closed chest" << std::endl;
                chest.getComponent<SpriteComponent>().Play("Inactive");
                chestScreen->toggle();

               }
               }
            }
            }

        if (inventoryScreen->isCurrentlyVisible()) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
            inventoryScreen->moveSelection(-inventoryScreen->getGridCols());
            break;
            case SDLK_DOWN:
            inventoryScreen->moveSelection(inventoryScreen->getGridCols());
            break;
            case SDLK_LEFT:
            inventoryScreen->moveSelection(-1);
            break;
            case SDLK_RIGHT:
            inventoryScreen->moveSelection(1);
            break;
            case SDLK_u: // Assuming 'U' key is used to use an item
            inventoryScreen->useSelectedItem();
            break;
            }
        }

        if (chestScreen->isCurrentlyVisible()) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
            chestScreen->moveSelection(-chestScreen->getTotalCols());
                break;
            case SDLK_DOWN:
                chestScreen->moveSelection(chestScreen->getTotalCols());
                break;
            case SDLK_LEFT:
                chestScreen->moveSelection(-1);
                break;
            case SDLK_RIGHT:
                chestScreen->moveSelection(1);
                break;
            case SDLK_m:
                //move object
                break;
            }
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
            int Start_centerY = (screenHeight - 2 * buttonHeight - 20) / 2 + 100;
            int Setting_centerY = ((screenHeight - 2 * buttonHeight - 20) / 2 + 100) + 60;
            int exitCenterY = Setting_centerY + buttonHeight + 20;

            //if click is within start button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Start_centerY && y < Start_centerY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
                isMenuOpen = false;
                isGameStarted = true;
            }
            //if click is within Setting button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Setting_centerY && y < Setting_centerY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
                isSettingsOpen = true;
                isMenuOpen = false;

            }
            // Check if click is within the Exit button boundary
            if (x > centerX && x < centerX + buttonWidth &&
                y > exitCenterY && y < exitCenterY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
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
                Mix_PlayChannel(-1,clickButton, 0);
                isInGameMenuOpen = false;
            }
            //if click is within Setting button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Setting_centerY && y < Setting_centerY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
                isSettingsOpen = true;
                isInGameMenuOpen = false;

            }
            // Check if click is within the Exit button boundary
            if (x > centerX && x < centerX + buttonWidth &&
                y > exitCenterY && y < exitCenterY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
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
            int Back_centerY = ((screenHeight - 2 * buttonHeight - 20) / 2 + 100) + 180;

            Setting::handleSliderEvent({event.button.x, event.button.y});

            //if click is within back button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Back_centerY && y < Back_centerY + buttonHeight) {
                if (isGameStarted) {
                Mix_PlayChannel(-1,clickButton, 0);
                isSettingsOpen = false;
                isInGameMenuOpen = true;
                }
                else if (!isGameStarted){
                Mix_PlayChannel(-1,clickButton, 0);
                isSettingsOpen = false;
                isMenuOpen = true;

                }
            }

            //if click is within Music boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Music_centerY && y < Music_centerY + buttonHeight) {
                // Toggle music state
                if (isMusic) { // Music is currently playing
                    Mix_PlayChannel(-1,clickButton, 0);
                    Mix_PauseMusic(); // Pause the music
                    isMusic = false; // Update the flag
                    Setting::volume_onoff(isMusic);

                } else { // Music is currently paused
                    Mix_PlayChannel(-1,clickButton, 0);
                    Mix_ResumeMusic(); // Resume the music
                    isMusic = true; // Update the flag
                    Setting::volume_onoff(isMusic);

                }
            }
            //if click is within Screen Dimension button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > ScreenDim_centerY && y < ScreenDim_centerY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
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
                Mix_PlayChannel(-1,clickButton, 0);
                player.getComponent<Stats>().set_health(50);
                player.getComponent<TransformComponent>().position = Vector2D(1400, 1100);
                timeElapsed.start();
                isGameOverOpen = false;
            }

            // Check if click is within exit button boundary
            if (x > exitButtonRect.x && x < exitButtonRect.x + exitButtonRect.w &&
                y > exitButtonRect.y && y < exitButtonRect.y + exitButtonRect.h) {
                Mix_PlayChannel(-1,clickButton, 0);
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
    if (!isMenuOpen && !isGameOverOpen && !isInGameMenuOpen && !isSettingsOpen)
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

            //Check if the player collides with the chest
            for (auto& ch : chests)
            {
                InteractComponent& interact = ch->getComponent<InteractComponent>();

                if(Collision::AABB(playerCol,ch->getComponent<ColliderComponent>().collider))
                {
                    //std::cout << "Hit chest" << std::endl;
                    player.getComponent<TransformComponent>().position = playerPos;
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
            assets->CreateProjectile(Vector2D(enemyPos.x, enemyPos.y), Vector2D(1, 0), 200, 2, "enemy_projectile",false,32,32,3);
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
        if (timeElapsed.getTimeStart() > 300000) {
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

    // If the DisplayMap flag is true, render only the tiles with the correct scaling
    if (!DisplayMap) {
        // Render all regular game objects when not in map view
        for (auto& t : tiles) { t->draw(); }
        for (auto& c : MapColliders) { c->draw(); }
        for (auto& p : players) { p->draw(); }
        for (auto& e : enemies) { e->draw(); }
        for (auto& pp : PlayerProjectiles) { pp->draw(); }
        for (auto& ch: chests) { ch->draw(); }
        for (auto& ep : EnemyProjectiles) { ep->draw(); }
        for (auto& p : PlayerAttacks) {p->draw();}

        // Render the UI elements over the game objects
        label.draw();
        enemy_health.draw();
        player_health.draw();

        TestCol.draw();



    } else {

        for (auto& t : tiles) {
            auto& tileComponent = t->getComponent<TileComponent>();
            tileComponent.setTileScale2(1); // Assuming scale2 is the desired scale for map display
            t->draw();
        }


        // Gets the scaled position of the player from the TransformComponent.
        Vector2D playerPosition = player.getComponent<TransformComponent>().position;

        // Adjust player's position for the scaling factor.
        Vector2D scaledPlayerPosition;
        scaledPlayerPosition.x = playerPosition.x / 3; // Assuming 'scale' is your scaling factor (3 in this case).
        scaledPlayerPosition.y = playerPosition.y / 3;

        // Set the drawing color to red for the dot.
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RGBA for red.

        // Define the size of the dot, considering the scale.
        const int dotSize = 10; // You might want to scale this size as well.

        // Calculate the rectangle where the dot will be drawn, centering it around the scaled player's position.
        SDL_Rect dotRect = {
            static_cast<int>(scaledPlayerPosition.x) - dotSize / 2,
            static_cast<int>(scaledPlayerPosition.y) - dotSize / 2,
            dotSize,
            dotSize
        };

        SDL_RenderFillRect(renderer, &dotRect);
    }

    inventoryScreen->render(renderer);
    chestScreen->render(renderer);


    SDL_RenderPresent(renderer);
    }
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
