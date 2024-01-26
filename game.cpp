#include "game.hpp"
#include <SDL.h>
#include "TextureManager.hpp"
#include "map.hpp"
#include "ECS/Components.hpp"
#include "ECS/Stats.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"
#include "setting.hpp"
#include "ingame_menu.hpp"
#ifdef __APPLE__
#include <CoreGraphics/CGDirectDisplay.h>
#endif
#ifdef _WIN32
#include <Windows.h>
#endif
#include <sstream>
#include <variant>
#include <filesystem>
#include <fstream>
#include "world.hpp"
#include "inventory.hpp"

Map* map;
Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

//Camera
SDL_Rect Game::camera = {0,0,1600,1280};
int x_diff = 400; //Camera.x with respect to the position x of the player
int y_diff = 320; //Camera.y with respect to the position y of the player

AssetManager* Game::assets = new AssetManager(&manager);
Inventory* Game::inventory = new Inventory();
ChestScreen* Game::chestScreen1 = new ChestScreen();
ChestScreen* Game::chestScreen2 = new ChestScreen();

bool Game::isRunning = false;
bool Game::DisplayMap = false;

int i;

int Game::windowSize_x = 100;
int Game::windowSize_y = 100;


//click Button sound


//Add characters
auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& player_health(manager.addEntity());
//auto& enemy(manager.addEntity());
//test second enemy
//auto& enemy2(manager.addEntity());
//test turret enemy
//auto& enemy3(manager.addEntity());
// Add chests
auto& chest(manager.addEntity());
auto& chest2(manager.addEntity());

//End

//Test collision with rotated objects
auto& TestCol(manager.addEntity());
//Test collision with cirecles
auto& TestCircle(manager.addEntity());

bool chest_open = false;

std::filesystem::path projectDir = std::filesystem::current_path();

bool playerInvincible = false;
int t = 0;
Uint32 playerInvincibleStartTime = 0; // the player invincibility start time
Uint32 playerInvincibleDuration = 3000; // 3000 milliseconds



Game::Game()
{
    isGameStarted = false ; // tracks if player has started the game yet
    isMenuOpen = true; // Menu status, starts with menu opened
    isInGameMenuOpen = false; //  status for in game menu, starts closed
    isSettingsOpen = false; // Menu status, starts with menu opened
    isGameOverOpen = false;
    isRuleOpen = false;
    isFullscreen = false; // full screen statusm, Starts fullscreen mode
    isMusic = true; // music state, music on by default

    Game::assets->manager->setGame(this);

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
        SDL_SetRenderDrawBlendMode(renderer,
                                       SDL_BLENDMODE_BLEND);

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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        isRunning = false;
    }
    if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) {
        std::cerr << "Mix_Init failed: " << Mix_GetError() << std::endl;
    }
    Mix_AllocateChannels(2);

    }


    map = new Map("terrain", 4, 32, &manager);
    Game::inventory->init();
    Game::chestScreen1->init();
    Game::chestScreen2->init();


    std::string setUpPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "World_1_setup.json").string();

    loadSetUpJSON(setUpPath);


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
    player.addComponent<ColliderComponentCircle>("player",50);
    player.addGroup(Game::groupPlayers);
    timeElapsed = Timer();
    timeElapsed.start();

    std::cout << "Player created" << std::endl;

    //Create enemies
    spawnEnemiesLvl1();

    /*//Enemy base definition

    enemy.addComponent<TransformComponent>(1200,1000,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    Stats& playerStats = player.getComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(2,500,200,1200,60,&playerTransform, &playerStats, &enemyStats); //To be changed later on
    enemy.addGroup(Game::groupEnemies);

    std::cout << "Enemy created" << std::endl;

    //End of Enemy base definition

    //create second enemy

    enemy2.addComponent<TransformComponent>(1300,1000,128,128,1);
    enemy2.addComponent<SpriteComponent>(true, "enemy");
    enemy2.getComponent<SpriteComponent>().setActions();
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<Stats>();
    enemy2.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy3.addComponent<TransformComponent>(2000,1000,128,128,1);
    enemy3.addComponent<SpriteComponent>(true, "enemy");
    enemy3.getComponent<SpriteComponent>().setActions();
    enemy3.addComponent<ColliderComponent>("enemy");
    enemy3.addComponent<Stats>();
    enemy3.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy3.addGroup(Game::groupEnemies);*/

    //create first chest

    chest.addComponent<TransformComponent>(900,900,16,16,5);
    chest.addComponent<SpriteComponent>(true, "chest");
    chest.getComponent<SpriteComponent>().setActions();
    chest.addComponent<ColliderComponent>("chest");
    chest.addComponent<InteractComponent>();
    chest.addComponent<ChestScreen>();
    chest.addGroup(Game::groupChests);

    //create second chest

    chest2.addComponent<TransformComponent>(1000,1200,16,16,5);
    chest2.addComponent<SpriteComponent>(true, "chest");
    chest2.getComponent<SpriteComponent>().setActions();
    chest2.addComponent<ColliderComponent>("chest");
    chest2.addComponent<InteractComponent>();
    chest2.addComponent<ChestScreen>();
    chest2.addGroup(Game::groupChests);
}


    //Create labels
    {
    SDL_Color white = {255,255,255,255};
    SDL_Color green = {0,255,0,255};
    SDL_Color red = {255,0,0,255};
    label.addComponent<UILabel>(10,10, "Test String", "arial", white, true);

    lastProjectileTime = SDL_GetTicks();
    }

    TestCol.addComponent<TransformComponent>(1700,1500,100,200);
    TestCol.addComponent<ColliderComponent>("terrain");
    TestCol.getComponent<ColliderComponent>().SetAngle(135);

    //Initialize all items as hide
    std::string handPath =  (projectDir / ".." / "TimelessJourneys" / "assets" / "hand.png").string();

    //inventory.addNewItem(Items* items,handPath,renderer);

    TestCircle.addComponent<TransformComponent>(1400,1500,200,200);
    TestCircle.addComponent<ColliderComponentCircle>("circle",100);

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
            inventory->toggle();  // Toggle the inventory screen
            }
            break;
        case SDLK_c:
            if (level == "lvl1")
            {
                loadLvl3();
                level = "lvl3";
            }
            else if (level == "lvl3")
            {
                loadLvl2();
                level = "lvl2";
            }
            else if (level == "lvl2")
            {
                loadLvl1();
                level= "lvl1";
            }
            break;
        case SDLK_RETURN:
            i = 1;
            ChestScreen *chestScreen;
            for (auto& ch : chests)
            {
            if (inventory->get_visibility() == false) {
                InteractComponent& interact = ch->getComponent<InteractComponent>();

               if (chest_open == false) {
                if (interact.PlayerCloseTo(player.getComponent<TransformComponent>()))
                {
                    std::cout << "Opened chest" << std::endl;
                    ch->getComponent<SpriteComponent>().Play("Active");
                    chest_open = true;
                    ch->getComponent<ChestScreen>().toggle();
                    break;
                }
               }
               else {
                if (interact.PlayerCloseTo(player.getComponent<TransformComponent>()))
                {
                chest_open = false;
                std::cout << "Closed chest" << std::endl;
                ch->getComponent<SpriteComponent>().Play("Inactive");
                ch->getComponent<ChestScreen>().toggle();
                break;
               }
               }
               }
            i++;
            }
            break;

        default:
            if (inventory -> get_visibility()) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
               inventory -> moveSelection(-inventory -> getGridCols());
               break;
            case SDLK_DOWN:
               inventory -> moveSelection(inventory -> getGridCols());
               break;
            case SDLK_LEFT:
               inventory -> moveSelection(-1);
               break;
            case SDLK_RIGHT:
               inventory -> moveSelection(1);
               break;

            case SDLK_u: // Assuming 'U' key is used to use an item
               inventory -> useSelectedItem();
               break;

            }
            }

            for (auto& ch : chests)
            {
            if (ch->getComponent<ChestScreen>().isCurrentlyVisible()) {
               std::cout << "chest is " << i << std::endl;
               switch (event.key.keysym.sym) {
               case SDLK_UP:
               std::cout << "going up" << std::endl;
               ch->getComponent<ChestScreen>().moveSelection(-(ch->getComponent<ChestScreen>().getTotalCols()));
               break;
               case SDLK_DOWN:
               ch->getComponent<ChestScreen>().moveSelection(ch->getComponent<ChestScreen>().getTotalCols());
               break;
               case SDLK_LEFT:
               ch->getComponent<ChestScreen>().moveSelection(-1);
               break;
               case SDLK_RIGHT:
               ch->getComponent<ChestScreen>().moveSelection(1);
               break;
               case SDLK_m:
               //move object
               ch->getComponent<ChestScreen>().moveItem();
               break;
               }
            }
            }
            break;
        }


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
            int Rules_centerY = Setting_centerY + buttonHeight + 20;
            int exitCenterY = Rules_centerY + buttonHeight + 20;

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
            //if click is within Rules button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Rules_centerY && y < Rules_centerY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
                isRuleOpen = true;
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
            int Rules_centerY = Setting_centerY + buttonHeight + 20;
            int exitCenterY = Rules_centerY + buttonHeight + 20;


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
            //if click is within Rules button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Rules_centerY && y < Rules_centerY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
                isRuleOpen = true;
                isInGameMenuOpen = false;

            }
            // Check if click is within the Exit button boundary
            if (x > centerX && x < centerX + buttonWidth &&
                y > exitCenterY && y < exitCenterY + buttonHeight) {
                Mix_PlayChannel(-1,clickButton, 0);
                isRunning = false;
            }
        }
        else if (isRuleOpen) {
            // Get the mouse coordinates and screen size
            int x, y ,screenWidth, screenHeight;
            SDL_GetMouseState(&x, &y);
            SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
            // Menu button dimensions
            int buttonWidth = 150;
            int buttonHeight = 40;
            // Calculating location of buttons
            int centerX = (screenWidth - buttonWidth) / 2;
            int backButtonCenterY = (screenHeight - 2 * buttonHeight - 20) / 2 + 260;
            //if click is within back button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > backButtonCenterY && y < backButtonCenterY + buttonHeight) {
                if (isGameStarted) {
                Mix_PlayChannel(-1,clickButton, 0);
                isRuleOpen = false;
                isInGameMenuOpen = true;
                }
                else if (!isGameStarted){
                Mix_PlayChannel(-1,clickButton, 0);
                isRuleOpen = false;
                isMenuOpen = true;

                }
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
    //Update window size
    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    Game::windowSize_x = w;
    Game::windowSize_y = h;
    //end

    if (!isMenuOpen && !isGameOverOpen && !isInGameMenuOpen && !isSettingsOpen)
    {
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
            if(Collision::CheckCollision(c->getComponent<ColliderComponent>(), player.getComponent<ColliderComponent>()))
            {
                std::cout << "Hit wall" << std::endl;
                player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move

            }

            for (auto& e : enemies)
            {
                if (e->hasComponent<EnemyMovement>())
                {
                    if (e->getComponent<EnemyMovement>().collisionCooldown > 0) continue;
                    if(Collision::CheckCollision(c->getComponent<ColliderComponent>(), e->getComponent<ColliderComponent>()))
                    {
                        std::cout << "Enemy hit wall" << std::endl;
                        e->getComponent<EnemyMovement>().onCollision(c->getComponent<ColliderComponent>().collider); // the enemy doesn't move
                    }
                }
            }
        }

        if(enemies.empty())
        {
            loadLvl2();
        }
        //End

        //Test collision with rotated objects
/*
        if (Collision::CheckCollision(TestCol.getComponent<ColliderComponent>() ,player.getComponent<ColliderComponent>()))
        {
            std::cout << "Player hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }
*/
        if (Collision::CollisionRectCircle(player.getComponent<ColliderComponent>(),TestCircle.getComponent<ColliderComponentCircle>()))
        {
            std::cout << "Player hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }
        if (Collision::CollisionRectCircle(TestCol.getComponent<ColliderComponent>(),player.getComponent<ColliderComponentCircle>()))
        {
            std::cout << "Player hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }
        //end

        for (auto& p : EnemyProjectiles)
        {
            if(Collision::CheckCollision(player.getComponent<ColliderComponent>(),p->getComponent<ColliderComponent>()))
            {
                std::cout << "Hit player!" << std::endl;
                p->getComponent<ProjectileComponent>().DoDamage(player.getComponent<Stats>());
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
                    playerInvincible = true;
                    playerInvincibleStartTime = currentTime;
                }
            }
            for (auto& p : PlayerProjectiles)
            {
                if(Collision::CheckCollision(p->getComponent<ColliderComponent>(),e->getComponent<ColliderComponent>()))
                {
                    std::cout << "Projectile hit enemy" << std::endl;
                    p->getComponent<ProjectileComponent>().DoDamage(player.getComponent<Stats>(),e->getComponent<Stats>());
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
                        player.getComponent<Sword>().DoDamage(player.getComponent<Stats>(),e->getComponent<Stats>());
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
    else if (isRuleOpen) {
    Rule::renderRule(renderer, isRuleOpen, mousePosition); // Render the ruleset if it's open
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
        for (auto& e : enemies) { e->draw(); }
        for (auto& ch: chests) { ch->draw(); }
        for (auto& p : players) { p->draw(); }
        for (auto& ep : EnemyProjectiles) { ep->draw(); }
        for (auto& p : PlayerAttacks) {p->draw();}
        for (auto& pp : PlayerProjectiles) { pp->draw(); }

        // Render the UI elements over the game objects
        label.draw();
        //Test collider
        TestCol.draw();
        //Test circle collider
        TestCircle.draw();

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


    inventory->render(renderer);
    chestScreen1->render(renderer);
    chestScreen2->render(renderer);

    //Render time remaining
    //TODO: set to screen size
    Uint32 timeRemaining = (300000 - timeElapsed.getTimeStart())/1000;
    int minutes = timeRemaining/60;
    int seconds = timeRemaining - minutes*60;
    std::string text;
    text += "0";
    text += std::to_string(minutes);
    text += ":";
    if(seconds<10)
    {
        text += "0";
    }
    text += std::to_string(seconds);
    SDL_Colour color = {0,0,0,255};
    if(minutes < 1)
    {
       color = {0,200,0,255};
    }
    SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont("arial"),text.c_str(),color);
    timeLabel = SDL_CreateTextureFromSurface(Game::renderer, surf);
    SDL_FreeSurface(surf);
    int windowHeight;
    int windowWidth;
    SDL_GetWindowSize(window,&windowWidth,&windowHeight);
    int shift_x = windowWidth - 320;
    int shift_y = 20;
    SDL_Rect destRect;
    destRect={shift_x,shift_y,310,120};
    SDL_RenderCopy(Game::renderer,assets->GetTexture("Oval"),NULL,&destRect);
    destRect={shift_x,shift_y-10,70,140};
    SDL_RenderCopy(Game::renderer,assets->GetTexture("Hourglass"),NULL,&destRect);
    destRect={shift_x+220,shift_y+15,90,90};
    SDL_RenderCopy(Game::renderer,assets->GetTexture("PocketWatch"),NULL,&destRect);
    destRect = {shift_x+70,shift_y+10,150,100};

    SDL_RenderCopy(Game::renderer,timeLabel,NULL,&destRect);

    //end
    SDL_RenderPresent(renderer);
    }
}

void Game::loadSetUpJSON(std::string path)
{
    std::ifstream jsonFileStream(path);
    nlohmann::json jsonData = nlohmann::json::parse(jsonFileStream);

    mapPath = projectDir.string() + jsonData["mapPath"].get<std::string>();
    fontPath = projectDir.string() + jsonData["fontPath"].get<std::string>();
    itemsPath = projectDir.string() + jsonData["itemsPath"].get<std::string>();
    musicPath = projectDir.string() + jsonData["musicPath"].get<std::string>();
    worldPath = projectDir.string() + jsonData["worldPath"].get<std::string>();

    for (int i = 0; i < size(jsonData["setUpSprites"]); ++i)
    {
    nlohmann::json spriteData = jsonData["setUpSprites"][i];
    std::string name = spriteData["name"].get<std::string>();
    std::string path = spriteData["path"].get<std::string>();
    assets->AddTexture(name,path.c_str());
    }

    assets->loadWorld(worldPath);
    assets->AddFont("arial",fontPath.c_str(),16);
    map->LoadMap(mapPath.c_str(),25,20);
    Game::inventory->loadFromJSON(itemsPath);


    std::cout << "Trying to load music from: " << musicPath << std::endl;
    bgMusic = Mix_LoadMUS(musicPath.c_str());
    if (!bgMusic) {
    std::cerr << "Failed to load background music from " << musicPath << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
    // Handle the error, maybe exit or provide a notification
    } else {
    if (Mix_PlayMusic(bgMusic, -1) == -1) {
       std::cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    }

}

void Game::spawnEnemiesLvl1()
{
    auto& enemy(manager.addEntity());
    auto& enemy2(manager.addEntity());
    auto& enemy3(manager.addEntity());

    //Enemy base definition

    enemy.addComponent<TransformComponent>(1200,1000,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    Stats& playerStats = player.getComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(2,500,200,1200,60,&playerTransform, &playerStats, &enemyStats); //To be changed later on
    enemy.addGroup(Game::groupEnemies);

    //create second enemy

    enemy2.addComponent<TransformComponent>(1300,1000,128,128,1);
    enemy2.addComponent<SpriteComponent>(true, "enemy");
    enemy2.getComponent<SpriteComponent>().setActions();
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<Stats>();
    enemy2.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy3.addComponent<TransformComponent>(2000,1000,128,128,1);
    enemy3.addComponent<SpriteComponent>(true, "enemy");
    enemy3.getComponent<SpriteComponent>().setActions();
    enemy3.addComponent<ColliderComponent>("enemy");
    enemy3.addComponent<Stats>();
    enemy3.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy3.addGroup(Game::groupEnemies);
}

void Game::loadLvl1()
{
    delete map;
    for (auto& t : tiles) { t->destroy(); }
    for (auto& c : MapColliders) { c->destroy(); }
    for (auto& e : enemies) { e->destroy(); }
    for (auto& ch : chests) { ch->destroy(); }

    manager.refresh();

    assets->AddTexture("terrain1", "/assets/terrainAssets/terrain_ss.png");
    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map" / "map.map").string();
    map = new Map("terrain1",4,32,&manager);
    map->LoadMap2(mapPath.c_str(),25,20);


    auto& enemy(manager.addEntity());
    auto& enemy2(manager.addEntity());
    auto& enemy3(manager.addEntity());
    auto& enemy4(manager.addEntity());

    //Enemy base definition

    enemy.addComponent<TransformComponent>(500,700,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    Stats& playerStats = player.getComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(2,500,200,1200,60,&playerTransform, &playerStats, &enemyStats); //To be changed later on
    enemy.addGroup(Game::groupEnemies);

    //create second enemy

    enemy2.addComponent<TransformComponent>(1300,1000,128,128,1);
    enemy2.addComponent<SpriteComponent>(true, "enemy");
    enemy2.getComponent<SpriteComponent>().setActions();
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<Stats>();
    enemy2.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy3.addComponent<TransformComponent>(2000,1000,128,128,1);
    enemy3.addComponent<SpriteComponent>(true, "enemy");
    enemy3.getComponent<SpriteComponent>().setActions();
    enemy3.addComponent<ColliderComponent>("enemy");
    enemy3.addComponent<Stats>();
    enemy3.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy3.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy4.addComponent<TransformComponent>(1000,1000,128,128,1);
    enemy4.addComponent<SpriteComponent>(true, "enemy");
    enemy4.getComponent<SpriteComponent>().setActions();
    enemy4.addComponent<ColliderComponent>("enemy");
    enemy4.addComponent<Stats>();
    enemy4.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy4.addGroup(Game::groupEnemies);

    camera.w = 3200 - screen_width;
    camera.h = 2560 - screen_height;
}

void Game::loadLvl2()
{
    delete map;
    for (auto& t : tiles) { t->destroy(); }
    for (auto& c : MapColliders) { c->destroy(); }
    for (auto& e : enemies) { e->destroy(); }
    for (auto& ch : chests) { ch->destroy(); }

    manager.refresh();

    assets->AddTexture("terrain2", "/assets/map/tileset-terrain.png");
    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map2.map").string();
    map = new Map("terrain2",4,32,&manager);
    map->LoadMap2(mapPath.c_str(),25,20);

    player.getComponent<TransformComponent>().position.x = 2044;
    player.getComponent<TransformComponent>().position.y = 480;

    auto& enemy(manager.addEntity());
    auto& enemy2(manager.addEntity());
    auto& enemy3(manager.addEntity());
    auto& enemy4(manager.addEntity());

    //Enemy base definition

    enemy.addComponent<TransformComponent>(500,700,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    Stats& playerStats = player.getComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(2,500,200,1200,60,&playerTransform, &playerStats, &enemyStats); //To be changed later on
    enemy.addGroup(Game::groupEnemies);

    //create second enemy

    enemy2.addComponent<TransformComponent>(1300,1000,128,128,1);
    enemy2.addComponent<SpriteComponent>(true, "enemy");
    enemy2.getComponent<SpriteComponent>().setActions();
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<Stats>();
    enemy2.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy3.addComponent<TransformComponent>(2000,1000,128,128,1);
    enemy3.addComponent<SpriteComponent>(true, "enemy");
    enemy3.getComponent<SpriteComponent>().setActions();
    enemy3.addComponent<ColliderComponent>("enemy");
    enemy3.addComponent<Stats>();
    enemy3.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy3.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy4.addComponent<TransformComponent>(1000,1000,128,128,1);
    enemy4.addComponent<SpriteComponent>(true, "enemy");
    enemy4.getComponent<SpriteComponent>().setActions();
    enemy4.addComponent<ColliderComponent>("enemy");
    enemy4.addComponent<Stats>();
    enemy4.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy4.addGroup(Game::groupEnemies);

    camera.w = 3200 - screen_width;
    camera.h = 2560 - screen_height;
}

void Game::loadLvl3()
{
    delete map;
    for (auto& t : tiles) { t->destroy(); }
    for (auto& c : MapColliders) { c->destroy(); }
    for (auto& e : enemies) { e->destroy(); }
    for (auto& ch : chests) { ch->destroy(); }

    manager.refresh();

    assets->AddTexture("terrain3", "/assets/terrainAssets/Map3tile_set.png");
    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map" / "Map3.map").string();
    map = new Map("terrain3",4,32,&manager);
    map->LoadMap3(mapPath.c_str(),50,40);

    player.getComponent<TransformComponent>().position.x = 700;
    player.getComponent<TransformComponent>().position.y = 532;

    auto& enemy(manager.addEntity());
    auto& enemy2(manager.addEntity());
    auto& enemy3(manager.addEntity());
    auto& enemy4(manager.addEntity());

    //Enemy base definition

    enemy.addComponent<TransformComponent>(500,700,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    Stats& playerStats = player.getComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(2,500,200,1200,60,&playerTransform, &playerStats, &enemyStats); //To be changed later on
    enemy.addGroup(Game::groupEnemies);

    //create second enemy

    enemy2.addComponent<TransformComponent>(1300,1000,128,128,1);
    enemy2.addComponent<SpriteComponent>(true, "enemy");
    enemy2.getComponent<SpriteComponent>().setActions();
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<Stats>();
    enemy2.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy3.addComponent<TransformComponent>(2000,1000,128,128,1);
    enemy3.addComponent<SpriteComponent>(true, "enemy");
    enemy3.getComponent<SpriteComponent>().setActions();
    enemy3.addComponent<ColliderComponent>("enemy");
    enemy3.addComponent<Stats>();
    enemy3.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy3.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy4.addComponent<TransformComponent>(1000,1000,128,128,1);
    enemy4.addComponent<SpriteComponent>(true, "enemy");
    enemy4.getComponent<SpriteComponent>().setActions();
    enemy4.addComponent<ColliderComponent>("enemy");
    enemy4.addComponent<Stats>();
    enemy4.addComponent<TurretEnemy>(400,5,5,400,&manager,&player.getComponent<TransformComponent>());
    enemy4.addGroup(Game::groupEnemies);

    camera.w = 6400 - screen_width;
    camera.h = 5120 - screen_height;
}

bool Game::isChestOpen()
{
    for (auto& ch : chests)
    {
        if(ch->getComponent<ChestScreen>().isCurrentlyVisible())
        {
            return true;
        }
    }
    return false;
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
