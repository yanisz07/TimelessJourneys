#ifndef UILABEL_H
#define UILABEL_H

#include "ECS.hpp"
#include "../AssetManager.hpp"
#include "../game.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

class UILabel : public Component
{
public:
    UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour, bool is_static) :
        labelText(text), labelFont(font), textColour(colour), Static(is_static)
    {
        position.x = xpos;
        position.y = ypos;

        SetLabelText(labelText, labelFont);
    }
    ~UILabel() override
    {
        SDL_DestroyTexture(labelTexture);
    }

    void SetLabelText(std::string text, std::string font)
    {
        SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), textColour);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
        SDL_FreeSurface(surf);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
    }

    void SetPositionText(int x, int y)
    {
        position.x = x;
        position.y = y;
    }

    void draw() override
    {
        if (Static)
        {
            SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
        }
        else
        {
            position.x -= Game::camera.x;
            position.y -= Game::camera.y;
            SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
        }
    }

private:

    bool Static = true;
    SDL_Rect position;
    std::string labelText;
    std::string labelFont;
    SDL_Color textColour;
    SDL_Texture* labelTexture;

};

#endif // UILABEL_H
