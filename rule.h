#ifndef RULE_H
#define RULE_H


#include <SDL.h>
#include <filesystem>

extern std::filesystem::path projectDir;

//this file + the menu.cpp is used to store de fucntions that handle opening and closing the menu.

class Rule {
public:
    static void toggleRuleState(bool &isMenuOpen);
    static void renderRule(SDL_Renderer* renderer, bool isRuleOpen,const SDL_Point& mousePosition);
};

#endif // RULE_H

