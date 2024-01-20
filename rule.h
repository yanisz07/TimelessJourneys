#ifndef RULE_H
#define RULE_H


#include <SDL.h>
#include <filesystem>
#include <SDL_ttf.h>

extern std::filesystem::path projectDir;

//this file + the menu.cpp is used to store de fucntions that handle opening and closing the menu.

class Rule {
public:
    static void toggleRuleState(bool &isMenuOpen);
    static void renderRule(SDL_Renderer* renderer, bool isRuleOpen,const SDL_Point& mousePosition);
    static void renderMultilineText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Color& color, int x, int y, int maxWidth, int maxHeight);
};

#endif // RULE_H

