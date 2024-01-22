#ifndef MENU_H
#define MENU_H
#include <SDL.h>
#include <filesystem>

extern std::filesystem::path projectDir;

//this file + the menu.cpp is used to store de fucntions that handle opening and closing the menu.

class Menu {
public:
    static void toggleMenuState(bool &isMenuOpen);
    static void renderMenu(SDL_Renderer* renderer, bool isMenuOpen,const SDL_Point& mousePosition);
};

#endif // MENU_H

