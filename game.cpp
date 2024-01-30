#include "game.hpp"
#include <SDL.h>
#include <SDL_image.h>
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
#include <cmath>
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

bool Game::isRunning = false;
bool Game::DisplayMap = false;

int Game::windowSize_x = 100;
int Game::windowSize_y = 100;




//Add characters
auto& player(manager.addEntity());
auto& label(manager.addEntity());

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
    isRuleOpen = false;
    isFullscreen = false; // full screen statusm, Starts fullscreen mode
    isMusic = true;
    isSound = true;// music state, music on by default

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
    camera.w = 6400 - screen_width;
    camera.h = 5120 - screen_height;
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
    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
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

    Game::inventory->init();
    Game::inventory->setGame(this);


    std::string setUpPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "World_1_setup.json").string();

    loadSetUpJSON(setUpPath);


    //Create player
    player.setType("player");
    player.addComponent<TransformComponent>(1900,1100,48,48,3,20);
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

    std::cout << "Player created" << std::endl;

    loadLvl3();

    //Create labels
    SDL_Color white = {0,0,0,255};
    SDL_Color green = {0,255,0,255};
    SDL_Color red = {255,0,0,255};
    label.addComponent<UILabel>(10,10, "Test String", "arial", white, true);

    lastProjectileTime = SDL_GetTicks();


    TestCol.addComponent<TransformComponent>(1900,1500,100,200);
    TestCol.addComponent<ColliderComponent>("terrain");
    TestCol.getComponent<ColliderComponent>().SetAngle(30); // SET UP ALWAYS IN DEGREES NOT RAD

    //Initialize all items as hide
    std::string handPath =  (projectDir / ".." / "TimelessJourneys" / "assets" / "hand.png").string();

    //inventory.addNewItem(Items* items,handPath,renderer);

    TestCircle.addComponent<TransformComponent>(1900,1500,200,200);
    TestCircle.addComponent<ColliderComponentCircle>("circle",100);

    std::cout << "Game initialized" << std::endl;

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& MapColliders(manager.getGroup(Game::groupMapColliders));
auto& PlayerProjectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& EnemyProjectiles(manager.getGroup(Game::groupEnemyProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& npcs(manager.getGroup(Game::groupNPC));
auto& PlayerAttacks(manager.getGroup(Game::groupPlayerAttack));
auto& chests(manager.getGroup(Game::groupChests));
auto& Turrets(manager.getGroup(Game::groupTurrets));
auto& Canons(manager.getGroup(Game::groupCanons));
auto& Spawners(manager.getGroup(Game::groupSpawners));



void Game::handleEvents()
{
    SDL_PollEvent(&event);
    std::string click_path = (projectDir / ".." / "TimelessJourneys" / "assets" / "soundAssets" /"click_button1.mp3").string();
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
            int Rules_centerY = Setting_centerY + buttonHeight + 20;
            int exitCenterY = Rules_centerY + buttonHeight + 20;

            //if click is within start button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Start_centerY && y < Start_centerY + buttonHeight) {
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isMenuOpen = false;
                isGameStarted = true;
                timeElapsed.start();
            }
            //if click is within Setting button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Setting_centerY && y < Setting_centerY + buttonHeight) {
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1, clickButton, 0);}
               isSettingsOpen = true;
               isMenuOpen = false;

            }
            //if click is within Rules button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Rules_centerY && y < Rules_centerY + buttonHeight) {
               if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isRuleOpen = true;
                isMenuOpen = false;

            }
            // Check if click is within the Exit button boundary
            if (x > centerX && x < centerX + buttonWidth &&
                y > exitCenterY && y < exitCenterY + buttonHeight) {
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
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
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isInGameMenuOpen = false;
            }
            //if click is within Setting button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Setting_centerY && y < Setting_centerY + buttonHeight) {
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isSettingsOpen = true;
                isInGameMenuOpen = false;

            }
            //if click is within Rules button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Rules_centerY && y < Rules_centerY + buttonHeight) {
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isRuleOpen = true;
                isInGameMenuOpen = false;

            }
            // Check if click is within the Exit button boundary
            if (x > centerX && x < centerX + buttonWidth &&
                y > exitCenterY && y < exitCenterY + buttonHeight) {
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
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
               if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isRuleOpen = false;
                isInGameMenuOpen = true;
                }
                else if (!isGameStarted){
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
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
            int Back_centerY = ((screenHeight - 2 * buttonHeight - 20) / 2 + 100) + 240;
            int SoundEffect_centerY = ((screenHeight - 2* buttonHeight - 20) / 2 + 100) + 180;

            Setting::handleSliderEvent({event.button.x, event.button.y});

            //if click is within back button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Back_centerY && y < Back_centerY + buttonHeight) {
                if (isGameStarted) {
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isSettingsOpen = false;
                isInGameMenuOpen = true;
                }
                else if (!isGameStarted){
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                isSettingsOpen = false;
                isMenuOpen = true;

                }
            }

            //if click is within Music boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > Music_centerY && y < Music_centerY + buttonHeight) {
                // Toggle music state
                if (isMusic) { // Music is currently playing
                if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                    Mix_PauseMusic(); // Pause the music
                    isMusic = false; // Update the flag
                    Setting::volume_onoff(isMusic);

                } else { // Music is currently paused
                    if (soundEffectsEnabled) {
               Mix_PlayChannel(-1,clickButton, 0);}
                    Mix_ResumeMusic(); // Resume the music
                    isMusic = true; // Update the flag
                    Setting::volume_onoff(isMusic);

                }
            }
            //if click is within Screen Dimension button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > ScreenDim_centerY && y < ScreenDim_centerY + buttonHeight) {
                if (soundEffectsEnabled) {
                    Mix_PlayChannel(-1,clickButton, 0);}
                toggleFullScreen();
            }
            // if click is within Sound button boundary:
            if (x > centerX && x < centerX + buttonWidth &&
                y > SoundEffect_centerY && y < SoundEffect_centerY + buttonHeight) {
                if (isSound) {
                    if (soundEffectsEnabled) {
               Mix_PlayChannel(-1, clickButton, 0);}
                    isSound = false;
                    Setting::toggleSoundEffects();
                } else {
                    if (soundEffectsEnabled) {
               Mix_PlayChannel(-1, clickButton, 0);}
                    isSound = true;
                    Setting::toggleSoundEffects();

                }

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
                if (soundEffectsEnabled) {
                    Mix_PlayChannel(-1,clickButton, 0);}
                player.getComponent<Stats>().set_health(100);
                loadLvl3();
                //player.getComponent<Stats>().set_health(50);
                //player.getComponent<TransformComponent>().position = Vector2D(1900, 1100);
                timeElapsed.start();
                isGameOverOpen = false;
            }

            // Check if click is within exit button boundary
            if (x > exitButtonRect.x && x < exitButtonRect.x + exitButtonRect.w &&
                y > exitButtonRect.y && y < exitButtonRect.y + exitButtonRect.h) {
                if (soundEffectsEnabled) {
                    Mix_PlayChannel(-1,clickButton, 0);}
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
        SDL_SetWindowSize(window,1600,1280); //Fix size
        //fix camera
        screen_width = 1600;
        screen_height = 1280;
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
    if (level=="lvl1" || level=="lvl2")
    {
        camera.w = 3200-screen_width;
        camera.h = 2560-screen_height;
    }
    if (level == "lvl3")
    {
        camera.w = 6400-screen_width;
        camera.h = 5120-screen_height;
    }
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

        if(level == "lvl3"){
            bool tf = true;
            for(auto& e : enemies){
                if(e->tag == "juan"){
                    tf = false;
                }
            }
            if(tf){
            auto& chest1(manager.addEntity());

            chest1.setTag("chest");
            chest1.addComponent<TransformComponent>(5404,2471,16,16,5);
            chest1.addComponent<SpriteComponent>(true, "chest");
            chest1.getComponent<SpriteComponent>().setActions();
            chest1.addComponent<ColliderComponent>("chest");
            chest1.addComponent<InteractComponent>();
            chest1.addComponent<ChestScreen>();
            chest1.addGroup(Game::groupChests);
            }
        }

        if(player.getComponent<TransformComponent>().position.x <= 4749 && player.getComponent<TransformComponent>().position.x >= 4690 &&
            player.getComponent<TransformComponent>().position.y <= 3843 && player.getComponent<TransformComponent>().position.y >= 3800)
        {
            loadLvl2();
            level = "lvl2";
        }

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
                        e->getComponent<EnemyMovement>().onCollision(c->getComponent<ColliderComponent>());
                    }
                }
            }
        }

        for (auto& c : Canons)
        {
            if(Collision::CheckCollision(c->getComponent<ColliderComponent>(), player.getComponent<ColliderComponent>()))
            {
                player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
            }
        }

        for (auto& t : Turrets)
        {
            if(Collision::CheckCollision(t->getComponent<ColliderComponent>(), player.getComponent<ColliderComponent>()))
            {
                std::cout << "Hit turret" << std::endl;
                player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
            }
        }
        //with npcs:
        /*
        for (auto& c : npcs)
        {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            for (auto& e : enemies)
            {
                if (e->getComponent<EnemyMovement>().collisionCooldown > 0) continue;

                SDL_Rect e_cCol = e->getComponent<ColliderComponent>().collider;
                if(Collision::AABB(e_cCol, cCol))
                {
                    std::cout << "Enemy hit wall" << std::endl;
                    e->getComponent<EnemyMovement>().onCollision(c->getComponent<ColliderComponent>().collider); // the enemy doesn't move
                }
            }
        }*/
        //End
        //End

        //Test collision with rotated objects
/*
        if (Collision::CheckCollision(TestCol.getComponent<ColliderComponent>() ,player.getComponent<ColliderComponent>()))
        {
            std::cout << "Player hit wall" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos; // the player doesn't move
        }

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
*/
        //end

        for (auto& e : enemies)
        {
            if (e->hasComponent<EnemyMovement>())
            {
                if (e->getComponent<EnemyMovement>().collisionCooldown > 0) continue;
                if (Collision::CollisionRectCircle(e->getComponent<ColliderComponent>(),TestCircle.getComponent<ColliderComponentCircle>()))
                {
                    std::cout << "Enemy hit circle" << std::endl;
                    e->getComponent<EnemyMovement>().onCollisionCircle(TestCircle.getComponent<ColliderComponentCircle>().transform,TestCircle.getComponent<ColliderComponentCircle>().center);
                }
                if (Collision::CheckCollision(TestCol.getComponent<ColliderComponent>(),e->getComponent<ColliderComponent>())){

                    std::cout << "Enemy hit inclined " << std::endl;
                    e->getComponent<EnemyMovement>().onCollision(TestCol.getComponent<ColliderComponent>());
                }
            }
        }

        for (auto& p : EnemyProjectiles)
        {
            if (p->hasComponent<ColliderComponent>())
            {
                if(Collision::CheckCollision(player.getComponent<ColliderComponent>(),p->getComponent<ColliderComponent>()))
                {
                    std::cout << "Hit player!" << std::endl;
                    p->getComponent<ProjectileComponent>().DoDamage(player.getComponent<Stats>(),player.getComponent<Armor>());
                    p->destroy();
                }
            }
            if (p->hasComponent<ColliderComponentCircle>())
            {
                if(Collision::CollisionRectCircle(player.getComponent<ColliderComponent>(),p->getComponent<ColliderComponentCircle>()))
                {
                    std::cout << "Hit player!" << std::endl;
                    p->getComponent<ProjectileComponent>().DoDamage(player.getComponent<Stats>(),player.getComponent<Armor>());
                    p->destroy();
                }
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
                bool enemy_hit_wall;

                SDL_Rect e_Col = e->getComponent<ColliderComponent>().collider;
                SDL_Rect test_col;
                test_col.w = e_Col.w;
                test_col.h = e_Col.h;
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
                                test_col.x = e_Col.x + direction.x*10;
                                test_col.y = e_Col.y + direction.y*10;

                                for(auto& c : MapColliders)
                                {
                                    if (Collision::AABB(c->getComponent<ColliderComponent>().collider,test_col))
                                    {
                                        enemy_hit_wall = true; // the player doesn't move
                                    }
                                }
                                if(!enemy_hit_wall)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*10;
                                    e->getComponent<TransformComponent>().position.y += direction.y*10;
                                }
                            }
                            else
                            {
                                test_col.x = e_Col.x + direction.x*20;
                                test_col.y = e_Col.y + direction.y*20;

                                for(auto& c : MapColliders)
                                {
                                    if (Collision::AABB(c->getComponent<ColliderComponent>().collider,test_col))
                                    {
                                        enemy_hit_wall = true; // the player doesn't move
                                    }
                                }
                                if(!enemy_hit_wall)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*20;
                                    e->getComponent<TransformComponent>().position.y += direction.y*20;
                                }
                            }
                        }
                        else if (delay <= 180)
                        {
                            if(!type)
                            {
                                test_col.x = e_Col.x + direction.x*5;
                                test_col.y = e_Col.y + direction.y*5;

                                for(auto& c : MapColliders)
                                {
                                    if (Collision::AABB(c->getComponent<ColliderComponent>().collider,test_col))
                                    {
                                        enemy_hit_wall = true; // the player doesn't move
                                    }
                                }
                                if(!enemy_hit_wall)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*5;
                                    e->getComponent<TransformComponent>().position.y += direction.y*5;
                                }
                            }
                            else
                            {
                                test_col.x = e_Col.x + direction.x*10;
                                test_col.y = e_Col.y + direction.y*10;

                                for(auto& c : MapColliders)
                                {
                                    if (Collision::AABB(c->getComponent<ColliderComponent>().collider,test_col))
                                    {
                                        enemy_hit_wall = true; // the player doesn't move
                                    }
                                }
                                if(!enemy_hit_wall)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*10;
                                    e->getComponent<TransformComponent>().position.y += direction.y*10;
                                }
                            }
                        }
                        else
                        {
                            if(!type)
                            {
                                test_col.x = e_Col.x + direction.x*2;
                                test_col.y = e_Col.y + direction.y*2;

                                for(auto& c : MapColliders)
                                {
                                    if (Collision::AABB(c->getComponent<ColliderComponent>().collider,test_col))
                                    {
                                        enemy_hit_wall = true; // the player doesn't move
                                    }
                                }
                                if(!enemy_hit_wall)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*2;
                                    e->getComponent<TransformComponent>().position.y += direction.y*2;
                                }
                            }
                            else
                            {
                                test_col.x = e_Col.x + direction.x*5;
                                test_col.y = e_Col.y + direction.y*5;

                                for(auto& c : MapColliders)
                                {
                                    if (Collision::AABB(c->getComponent<ColliderComponent>().collider,test_col))
                                    {
                                        enemy_hit_wall = true; // the player doesn't move
                                    }
                                }
                                if(!enemy_hit_wall)
                                {
                                    e->getComponent<TransformComponent>().position.x += direction.x*5;
                                    e->getComponent<TransformComponent>().position.y += direction.y*5;
                                }
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

        //damage to archers on turrets

        for (auto& t : Turrets)
        {
            for (auto& pp : PlayerProjectiles)
            {
                if(Collision::CheckCollision(pp->getComponent<ColliderComponent>(),t->getComponent<ColliderComponent>()))
                {
                    std::cout << "Projectile hit enemy" << std::endl;
                    pp->getComponent<ProjectileComponent>().DoDamage(player.getComponent<Stats>(),t->getComponent<Stats>());
                    pp->destroy();
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
        Setting::renderSetting(renderer, isSettingsOpen, mousePosition, isFullscreen, isMusic, isSound); // Render the setting menu if it's open
    }
    else{

    int screenWidth, screenHeight;
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
    SDL_Color textColor = {255, 255, 255}; // White color for the text


    if (!DisplayMap) {
        // Render all regular game objects when not in map view
        for (auto& t : tiles) { t->draw(); }
        for (auto& c : MapColliders) { c->draw(); }
        for (auto& s : Spawners) {s->draw();}
        for (auto& ch: chests) { ch->draw(); }
        for (auto& t : Turrets) {t->draw();}
        for (auto& c : Canons) {c->draw();}
        for (auto& n : npcs) {n->draw(); }
        for (auto& e : enemies) { e->draw(); }
        for (auto& p : players) { p->draw(); }
        for (auto& ep : EnemyProjectiles) { ep->draw(); }
        for (auto& pp : PlayerProjectiles) { pp->draw(); }


        // Render the UI elements over the game objects
        label.draw();
        //Test collider
        //TestCol.draw();
        //Test circle collider
        //TestCircle.draw();
    }
     else {

        if (isFullscreen) {
                RenderFullscreenMap( renderer, screenWidth, screenHeight, map);
                renderPlayerPosition(renderer);

        // Gets the scaled position of the player from the TransformComponent.
        Vector2D playerPosition = player.getComponent<TransformComponent>().position;
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


    inventory->render(renderer);

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

void Game::spawnEnemies(std::string JSONpath)
{
    std::ifstream inFile(JSONpath);
    nlohmann::json j = nlohmann::json::parse(inFile);

    // Load items from JSON
    for (int i = 0; i < size(j["enemies"]); ++i) {
    nlohmann::json enemyData = j["enemies"][i];
    std::string type = enemyData["type"].get<std::string>();

    //Transform Comp
    int x_pos  = enemyData["trans"][0].get<int>();
    int y_pos  = enemyData["trans"][1].get<int>();
    int h = enemyData["trans"][2].get<int>();
    int w = enemyData["trans"][3].get<int>();
    int sc = enemyData["trans"][4].get<int>();


    //sprite component
    int animated = enemyData["sprite"][0].get<int>();
    std::string t = enemyData["sprite"][1].get<std::string>();

    //collider
    std::string collider = enemyData["coll"].get<std::string>();

    auto& enemy(manager.addEntity());

     enemy.addComponent<TransformComponent>(x_pos,y_pos,h,w,sc);
     enemy.addComponent<SpriteComponent>(animated, t);
     enemy.getComponent<SpriteComponent>().setActions();
     enemy.addComponent<ColliderComponent>(collider);

     if(type != "spaw" && type!="can"){
       enemy.addComponent<Stats>();}

     if(type == "turr")
     {
       int rad = enemyData["turr"][0].get<int>();
       int speed = enemyData["turr"][1].get<int>();
       int dam = enemyData["turr"][2].get<int>();
       int rt = enemyData["turr"][3].get<int>();
       enemy.addComponent<TurretEnemy>(rad,speed,dam,rt,&manager,&(player.getComponent<TransformComponent>()));
       enemy.addGroup(Game::groupTurrets);
     }
     else if(type == "can")
     {
       int rad = enemyData["can"][0].get<int>();
       int speed = enemyData["can"][1].get<int>();
       int dam = enemyData["can"][2].get<int>();
       int rt = enemyData["can"][3].get<int>();
       int dir = enemyData["can"][4].get<int>();
       enemy.addComponent<Canon>(rad,speed,dam,rt,&manager,&(player.getComponent<TransformComponent>()),dir);
       enemy.addGroup(Game::groupCanons);
     }
     else if(type == "spaw")
     {
       int inter = enemyData["spaw"][0].get<int>();
       int max_e = enemyData["spaw"][1].get<int>();
       enemy.getComponent<SpriteComponent>().Set_Dest_Rect(100,200);
       enemy.getComponent<SpriteComponent>().setActions();
       enemy.addComponent<SpawnerComponent>(manager, inter, max_e, &(player.getComponent<TransformComponent>()),
                                            &(player.getComponent<Stats>()), &(player.getComponent<ColliderComponent>()),&(player.getComponent<Armor>()));
       enemy.addGroup(Game::groupSpawners);
     }
     else if(type == "mov")
     {
      int ty =  enemyData["mov"][0].get<int>();
      int r_1 = enemyData["mov"][1].get<int>();
      int r_2 = enemyData["mov"][2].get<int>();
      int r_3 = enemyData["mov"][3].get<int>();
      int d_1 = enemyData["mov"][4].get<int>();
      int dam = enemyData["mov"][5].get<int>();
      enemy.addComponent<EnemyMovement>(ty,r_1,r_2,r_3,d_1,dam,&(player.getComponent<TransformComponent>()),
                                        &(player.getComponent<Stats>()), &(enemy.getComponent<Stats>()),&(player.getComponent<ColliderComponent>()),&(player.getComponent<Armor>()));
      enemy.addGroup(Game::groupEnemies);
     }
     else
     {
      enemy.addGroup(Game::groupEnemies);
      enemy.addGroup(Game::groupMapColliders);
     }

     std::cout << "Enemy " << type << " created" << std::endl;

    }
}

void Game::loadSetUpJSON(std::string path)
{
    std::ifstream jsonFileStream(path);
    nlohmann::json jsonData = nlohmann::json::parse(jsonFileStream);

    fontPath = projectDir.string() + jsonData["fontPath"].get<std::string>();
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
    std::cout << "World loaded" << std::endl;
    assets->AddFont("arial",fontPath.c_str(),16);
    std::cout << "Font added" << std::endl;


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

void Game::loadItems(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    nlohmann::json j = nlohmann::json::parse(inFile);

    // Clear current inventory
    Game::inventory->clearInventory();

    // Load items from JSON
    for (int i = 0; i < size(j["items"]); ++i) {
    nlohmann::json itemData = j["items"][i];
    std::string name = itemData["name"].get<std::string>();
    std::string type = itemData["type"].get<std::string>();
    std::string location = itemData["location"].get<std::string>();
    std::string spritePath = itemData["spritePath"].get<std::string>();
    float dmg_multiplier = itemData["dmg_multiplier"].get<float>();

    if (type=="Melee") {

       Item* item = new Melee(false,location,spritePath,name,dmg_multiplier);
       if(location == "inventory")
       {
            Game::inventory->addItem(item);

       }
       else
       {
            for (auto& ch : chests)
            {
                if(ch->tag == location)
                {
                    ch->getComponent<ChestScreen>().addItem(item);
                }
            }
       }
    }
    if (type=="RangedWeapon") {
       int range = itemData["range"].get<int>();
       int speed_arrow = itemData["speed_arrow"].get<int>();
       int speed_shooting = itemData["speed_shooting"].get<int>();
       Item* item = new RangedWeapon(false,location,spritePath,name,dmg_multiplier,range,speed_arrow,speed_shooting);
       if(location == "inventory")
       {
            Game::inventory->addItem(item);

       }
       else
       {
            for (auto& ch : chests)
            {
                if(ch->tag == location)
                {
                    ch->getComponent<ChestScreen>().addItem(item);
                }
            }
       }
    }
    if (type=="Armor") {
       int health_increase = itemData["health_increase"].get<int>();
       Item* item = new ArmorItem(false,location,spritePath,name,dmg_multiplier,health_increase);
       if(location == "inventory")
       {
            Game::inventory->addItem(item);

       }
       else
       {
            for (auto& ch : chests)
            {
                if(ch->tag == location)
                {
                    ch->getComponent<ChestScreen>().addItem(item);
                }
            }
       }
    }
    if (type=="HealingPotion") {
       int health_increase = itemData["health_increase"].get<int>();
       Item* item = new HealingPotion(false,location,spritePath,name,health_increase);
       if(location == "inventory")
       {
            Game::inventory->addItem(item);

       }
       else
       {
            for (auto& ch : chests)
            {
                if(ch->tag == location)
                {
                    ch->getComponent<ChestScreen>().addItem(item);
                }
            }
       }
    }

    if(type == "XPPotion")
    {
       int experience_increase = itemData["experience_increase"].get<int>();
       Item* item = new XPPotion(false,location,spritePath,name,experience_increase);
       if(location == "inventory")
       {
            Game::inventory->addItem(item);

       }
       else
       {
            for (auto& ch : chests)
            {
                if(ch->tag == location)
                {
                    ch->getComponent<ChestScreen>().addItem(item);
                }
            }
       }
    }


    std::cout << "Item " << name << "created and added" << std::endl;
    }

}


void Game::loadLvl1()
{
    delete map;
    for (auto& t : tiles) { t->destroy(); }
    for (auto& c : MapColliders) { c->destroy(); }
    for (auto& e : enemies) { e->destroy(); }
    for (auto& ch : chests) { ch->destroy(); }
    for (auto& ch : Turrets) { ch->destroy(); }
    for (auto& ch : Canons) { ch->destroy(); }
    for (auto& ch : Spawners) { ch->destroy(); }
    for (auto& n : npcs) {n->destroy();}

    manager.refresh();

    assets->AddTexture("terrain1", "/assets/terrainAssets/terrain_ss.png");
    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map.map").string();
    map = new Map("terrain1",4,32,&manager);
    map->LoadMap(mapPath.c_str(),25,20);

    player.getComponent<TransformComponent>().position.x = 1400;
    player.getComponent<TransformComponent>().position.y = 1100;

    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    ColliderComponent& playerCol = player.getComponent<ColliderComponent>();
    Stats& playerStats = player.getComponent<Stats>();
    Armor& playerArmor = player.getComponent<Armor>();

    auto& enemy(manager.addEntity());
    auto& enemy2(manager.addEntity());
    auto& enemy3(manager.addEntity());
    auto& spawner(manager.addEntity());
    auto& canon(manager.addEntity());
    auto& canon1(manager.addEntity());
    auto& chest1(manager.addEntity());
    auto& npc1(manager.addEntity());
    auto& npc2(manager.addEntity());
    auto& npc3(manager.addEntity());

    chest1.setTag("chest1");
    chest1.addComponent<TransformComponent>(900,900,16,16,5);
    chest1.addComponent<SpriteComponent>(true, "chest");
    chest1.getComponent<SpriteComponent>().setActions();
    chest1.addComponent<ColliderComponent>("chest");
    chest1.addComponent<InteractComponent>();
    chest1.addComponent<ChestScreen>();
    chest1.addGroup(Game::groupChests);

    //Enemy base definition

    enemy.addComponent<TransformComponent>(1200,1000,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();

    enemy.addComponent<EnemyMovement>(2,500,200,1200,60,3,&playerTransform, &playerStats, &enemyStats, &playerCol, &playerArmor); //To be changed later on
    enemy.addGroup(Game::groupEnemies);

    //create second enemy

    enemy2.addComponent<TransformComponent>(1300,1000,128,128,1);
    enemy2.addComponent<SpriteComponent>(true, "enemy");
    enemy2.getComponent<SpriteComponent>().setActions();
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<Stats>();
    enemy2.addGroup(Game::groupEnemies);

    //create turret enemy

    enemy3.addComponent<TransformComponent>(2000,1000,48,48,2);
    enemy3.addComponent<SpriteComponent>(true, "archer");
    enemy3.getComponent<SpriteComponent>().setActions();
    enemy3.addComponent<ColliderComponent>("turret");
    enemy3.addComponent<Stats>();
    enemy3.addComponent<TurretEnemy>(800,5,5,2000,&manager,&player.getComponent<TransformComponent>());
    enemy3.addGroup(Game::groupTurrets);

    //create spawner
    spawner.addComponent<TransformComponent>(1200,1000,500,250,1);
    spawner.addComponent<SpriteComponent>(true,"spawner");
    spawner.getComponent<SpriteComponent>().Set_Dest_Rect(100,200);
    spawner.getComponent<SpriteComponent>().setActions();
    spawner.addComponent<SpawnerComponent>(manager, 8000, 4, &playerTransform, &playerStats, &playerCol, &playerArmor);
    spawner.addGroup(Game::groupSpawners);


    //create RightCanon
    canon.addComponent<TransformComponent>(1500,500,24,24,4);
    canon.addComponent<SpriteComponent>(true, "Canon_Right");
    canon.getComponent<SpriteComponent>().setActions();
    canon.addComponent<ColliderComponent>("canon");
    canon.addComponent<Canon>(1000,7,10,4000,&manager,&player.getComponent<TransformComponent>(),0);
    canon.addGroup(Game::groupCanons);


    //create LeftCanon
    canon1.addComponent<TransformComponent>(1900,300,24,24,4);
    canon1.addComponent<SpriteComponent>(true, "Canon_Left");
    canon1.getComponent<SpriteComponent>().setActions();
    canon1.addComponent<ColliderComponent>("canon");
    canon1.addComponent<Canon>(1000,7,10,4000,&manager,&player.getComponent<TransformComponent>(),1);
    canon1.addGroup(Game::groupCanons);

    //creates npc
    npc1.addComponent<TransformComponent>(1200,800,128,128,1);
    npc1.addComponent<SpriteComponent>(true,"player");
    npc1.getComponent<SpriteComponent>().setActions();
    npc1.addComponent<NPCBehavior>(150,&playerTransform,"we count on you adventurer");
    npc1.addComponent<ColliderComponent>("terrain",1200+32,800+32,64,64);
    npc1.addGroup(Game::groupNPC);

    std::cout << "npc1 created" << std::endl;

    npc2.addComponent<TransformComponent>(1500,800,128,128,1);
    npc2.addComponent<SpriteComponent>(true,"player");
    npc2.getComponent<SpriteComponent>().setActions();
    npc2.addComponent<NPCBehavior>(150,&playerTransform,"we believe in you");
    npc2.addComponent<ColliderComponent>("terrain",1500+32,800+32,64,64);
    npc2.addGroup(Game::groupNPC);

    std::cout << "npc2 created" << std::endl;

    npc3.addComponent<TransformComponent>(1370,570,128,128,1);
    npc3.addComponent<SpriteComponent>(true,"player");
    npc3.getComponent<SpriteComponent>().setActions();
    npc3.addComponent<NPCBehavior>(150,&playerTransform,"we count on you adventurer");
    npc3.addComponent<ColliderComponent>("terrain",1370+32,570+32,64,64);
    npc3.addGroup(Game::groupNPC);

    std::cout << "npc3 created" << std::endl;

    itemsPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "items_Lvl1.json").string();
    loadItems(itemsPath);

    camera.w = 3200 - screen_width;
    camera.h = 2560 - screen_height;
    x_diff = (screen_width - 128)/2;
    y_diff = (screen_height - 128)/2;
}
void Game::loadLvl2()
{
    delete map;
    for (auto& t : tiles) { t->destroy(); }
    for (auto& c : MapColliders) { c->destroy(); }
    for (auto& e : enemies) { e->destroy(); }
    for (auto& ch : chests) { ch->destroy(); }
    for (auto& ch : Turrets) { ch->destroy(); }
    for (auto& ch : Canons) { ch->destroy(); }
    for (auto& ch : Spawners) { ch->destroy(); }
    for (auto& n : npcs) {n->destroy();}

    manager.refresh();

    //camera stuff
    camera.x = 0;
    camera.y = 0;
    camera.w = 3200 - screen_width;
    camera.h = 2560 - screen_height;
    x_diff = (screen_width - 128)/2;
    y_diff = (screen_height - 128)/2;

    assets->AddTexture("terrain2", "/assets/map/tileset-terrain.png");
    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map2.map").string();
    map = new Map("terrain2",4,32,&manager);
    map->LoadMap2(mapPath.c_str(),25,20);

    player.getComponent<TransformComponent>().position.x = 1717;
    player.getComponent<TransformComponent>().position.y = 500;

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


    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    ColliderComponent& playerCol = player.getComponent<ColliderComponent>();
    Stats& playerStats = player.getComponent<Stats>();

    auto& chest1(manager.addEntity());

    chest1.setTag("chest1");
    chest1.addComponent<TransformComponent>(2037,500,16,16,5);
    chest1.addComponent<SpriteComponent>(true, "chest");
    chest1.getComponent<SpriteComponent>().setActions();
    chest1.addComponent<ColliderComponent>("chest");
    chest1.addComponent<InteractComponent>();
    chest1.addComponent<ChestScreen>();
    chest1.addGroup(Game::groupChests);
    chest1.addGroup(Game::groupMapColliders);

}

void Game::loadLvl3()
{
    delete map;
    for (auto& t : tiles) { t->destroy(); }
    for (auto& c : MapColliders) { c->destroy(); }
    for (auto& e : enemies) { e->destroy(); }
    for (auto& ch : chests) { ch->destroy(); }
    for (auto& ch : Turrets) { ch->destroy(); }
    for (auto& ch : Canons) { ch->destroy(); }
    for (auto& ch : Spawners) { ch->destroy(); }
    for (auto& n : npcs) {n->destroy();}
    for (auto& n : PlayerProjectiles) {n->destroy();}
    for (auto& n : EnemyProjectiles) {n->destroy();}
    for (auto& n : PlayerAttacks) {n->destroy();}

    manager.refresh();
    //camera stuff
    camera.x = 0;
    camera.y = 0;
    camera.w = 6400 - screen_width;
    camera.h = 5120 - screen_height;
    x_diff = (screen_width - 128)/2;
    y_diff = (screen_height - 128)/2;

    //assets->AddTexture("terrain3", "/assets/terrainAssets/Map3tile_set.png");
    std::string mapPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "map" / "Map3.map").string();
    map = new Map("terrain3",4,32,&manager);
    map->LoadMap3(mapPath.c_str(),50,40);

    player.getComponent<TransformComponent>().position.x = 2440;
    player.getComponent<TransformComponent>().position.y = 4527;

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

    manager.update();

    TransformComponent& playerTransform = player.getComponent<TransformComponent>();
    ColliderComponent& playerCol = player.getComponent<ColliderComponent>();
    Stats& playerStats = player.getComponent<Stats>();

    auto& chest1(manager.addEntity());
    auto& chest2(manager.addEntity());

    auto& npc1(manager.addEntity());
    auto& npc2(manager.addEntity());
    auto& npc3(manager.addEntity());
    auto& npc4(manager.addEntity());

    //creates npc
    npc1.addComponent<TransformComponent>(1985,4452,128,128,1);
    npc1.addComponent<SpriteComponent>(true,"player");
    npc1.getComponent<SpriteComponent>().setActions();
    npc1.addComponent<NPCBehavior>(150,&playerTransform,"we count on you adventurer");
    npc1.addComponent<ColliderComponent>("terrain",1200+32,800+32,64,64);
    npc1.addGroup(Game::groupNPC);

    std::cout << "npc1 created" << std::endl;

    npc2.addComponent<TransformComponent>(1460,4452,128,128,1);
    npc2.addComponent<SpriteComponent>(true,"player");
    npc2.getComponent<SpriteComponent>().setActions();
    npc2.addComponent<NPCBehavior>(150,&playerTransform,"we believe in you");
    npc2.addComponent<ColliderComponent>("terrain",1500+32,800+32,64,64);
    npc2.addGroup(Game::groupNPC);

    std::cout << "npc2 created" << std::endl;

    npc3.addComponent<TransformComponent>(5060,4069,128,128,1);
    npc3.addComponent<SpriteComponent>(true,"player");
    npc3.getComponent<SpriteComponent>().setActions();
    npc3.addComponent<NPCBehavior>(150,&playerTransform,"we count on you adventurer");
    npc3.addComponent<ColliderComponent>("terrain",1370+32,570+32,64,64);
    npc3.addGroup(Game::groupNPC);

    std::cout << "npc3 created" << std::endl;

    npc4.addComponent<TransformComponent>(3512,1001,128,128,1);
    npc4.addComponent<SpriteComponent>(true,"player");
    npc4.getComponent<SpriteComponent>().setActions();
    npc4.addComponent<NPCBehavior>(150,&playerTransform,"we count on you adventurer");
    npc4.addComponent<ColliderComponent>("terrain",1370+32,570+32,64,64);
    npc4.addGroup(Game::groupNPC);

    std::cout << "npc4 created" << std::endl;

    chest1.setTag("chest1");
    chest1.addComponent<TransformComponent>(5290,3899,16,16,5);
    chest1.addComponent<SpriteComponent>(true, "chest");
    chest1.getComponent<SpriteComponent>().setActions();
    chest1.addComponent<ColliderComponent>("chest");
    chest1.addComponent<InteractComponent>();
    chest1.addComponent<ChestScreen>();
    chest1.addGroup(Game::groupChests);

    chest2.setTag("chest2");
    chest2.addComponent<TransformComponent>(5808,1460,16,16,5);
    chest2.addComponent<SpriteComponent>(true, "chest");
    chest2.getComponent<SpriteComponent>().setActions();
    chest2.addComponent<ColliderComponent>("chest");
    chest2.addComponent<InteractComponent>();
    chest2.addComponent<ChestScreen>();
    chest2.addGroup(Game::groupChests);


    auto enemiesPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "enemiesLvl3.json").string();

    spawnEnemies(enemiesPath);

    enemies[10]->setTag("juan");

    itemsPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "items_Lvl3.json").string();
    loadItems(itemsPath);


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
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    std::stringstream ss;
    ss << "Player Position: X=" << playerPos.x << ", Y=" << playerPos.y;

    SDL_Surface* textSurface = TTF_RenderText_Blended(Game::assets->GetFont("arial"), ss.str().c_str(), {255, 255, 255, 255});

    if (textSurface) {
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    const int padding = 5;
    SDL_Rect textBackgroundRect = {
        10 - padding,
        screen_height - textSurface->h - 30 - padding,
        textSurface->w + 2 * padding,
        textSurface->h + 2 * padding
    };

    SDL_Color borderColor = {50, 40, 30, 255}; // Dark gray/brownish tone
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderFillRect(renderer, &textBackgroundRect);

    // Draw inner background (same style as legend)
    SDL_Color backgroundColor = {120, 110, 100, 255}; // Lighter tone for contrast
    SDL_Rect innerBackgroundRect = {
        textBackgroundRect.x + padding,
        textBackgroundRect.y + padding,
        textBackgroundRect.w - 2 * padding,
        textBackgroundRect.h - 2 * padding
    };
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &innerBackgroundRect);

    // Render the text over the background
    SDL_Rect textRect = {
        10,
        screen_height - 30 - textSurface->h,
        textSurface->w,
        textSurface->h
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Clean up resources
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
    inventory->init();
    assets->free();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
