#ifndef INGAME_MENU_H
#define INGAME_MENU_H
#include <SDL.h>
#include <filesystem>

extern std::filesystem::path projectDir;

class InGameMenu {
public:
    static void toggleInGameMenuState(bool &isInGameMenuOpen);
    static void renderInGameMenu(SDL_Renderer* renderer, bool isInGameMenuOpen, const SDL_Point& mousePosition);
};

#endif // INGAME_MENU_H
