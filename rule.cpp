#include "rule.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <sstream>
#include <filesystem>


//brief
//FCHGDFB
void Rule::toggleRuleState(bool &isRuleOpen) {
    isRuleOpen = !isRuleOpen;
}

void Rule::renderRule(SDL_Renderer* renderer, bool isRuleOpen, const SDL_Point& mousePosition) {
    int titleButtonWidth = 300;
    int titleButtonHeight = 60;
    int ruleButtonWidth = 300;
    int ruleButtonHeight = 400;
    int backButtonWidth = 150;
    int backButtonHeight = 40;
    int borderThickness = 2;

    if (isRuleOpen) {
        int screenWidth, screenHeight;
        SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

        int titleCenterX = (screenWidth - titleButtonWidth) / 2;
        int titleCenterY = 50;
        int centerX = (screenWidth - ruleButtonWidth) / 2;
        int centerY = (screenHeight - 2 * ruleButtonHeight - 20) / 2 + 200;

        // Background
        std::string backgroundPath = (projectDir / ".." / "TimelessJourneys" / "assets" /"displayAssets"/  "background_sample1.png").string();
        SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, backgroundPath.c_str());
        SDL_Rect backgroundRect = {0, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_Rect titleButtonBorder = {titleCenterX - borderThickness, titleCenterY - borderThickness, titleButtonWidth + borderThickness * 2, titleButtonHeight + borderThickness * 2};
        SDL_Rect ruleButtonBorder = {centerX - borderThickness, centerY - borderThickness, ruleButtonWidth + borderThickness * 2, ruleButtonHeight + borderThickness * 2};
        SDL_Rect backButtonBorder = {centerX - borderThickness, centerY + ruleButtonHeight + 20 - borderThickness, ruleButtonWidth + borderThickness * 2, backButtonHeight + borderThickness * 2};

        SDL_RenderDrawRect(renderer, &titleButtonBorder);
        SDL_RenderDrawRect(renderer, &ruleButtonBorder);
        SDL_RenderDrawRect(renderer, &backButtonBorder);

        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // vanilla color

        SDL_Rect titleButton = {titleCenterX, titleCenterY, titleButtonWidth, titleButtonHeight};
        SDL_Rect ruleButton = {centerX, centerY, ruleButtonWidth, ruleButtonHeight};
        SDL_Rect backButton = {centerX, centerY + ruleButtonHeight + 20, ruleButtonWidth, backButtonHeight};

        bool isHoveringBack = SDL_PointInRect(&mousePosition, &backButton);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for borders
        SDL_RenderDrawRect(renderer, &titleButton);
        SDL_RenderDrawRect(renderer, &ruleButton);
        SDL_RenderDrawRect(renderer, &backButton);

        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Beige color for buttons
        SDL_RenderFillRect(renderer, &titleButton);

        // Set color for rule button background
        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Set to desired color for rule button background
        SDL_RenderFillRect(renderer, &ruleButton); // Fill the rule button background


        // hover effect for backs button
        if (isHoveringBack) {
            SDL_SetRenderDrawColor(renderer, 253, 254, 224, 255); // Lighter color for hover
        } else {
            SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Original color
        }
        SDL_RenderFillRect(renderer, &backButton);

        // Text Rendering
        std::string fontPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Arial.ttf").string();
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 24);
        SDL_Color textColor = {0, 0, 0, 255};


        // Render Title Text
        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Rules Book", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        int titleTextWidth, titleTextHeight;
        TTF_SizeText(font, "Rules Book", &titleTextWidth, &titleTextHeight);
        SDL_Rect titleTextRect = {titleCenterX + (titleButtonWidth - titleTextWidth) / 2, titleCenterY + (titleButtonHeight - titleTextHeight) / 2, titleTextWidth, titleTextHeight};
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleTextRect);

        // Render rule Text
        std::string rulesText = "\n Controls : \n\n\n    -Z: attack\n    -A: switch range/melee\n    -E: open inventory\n    -Escape: opens menu\n    -M: opens map\n    -Arrows: movement";
        renderMultilineText(renderer, font, rulesText, textColor, centerX, centerY, ruleButtonWidth, ruleButtonHeight);

        // Render back Text
        SDL_Surface* backSurface = TTF_RenderText_Solid(font, "Back", textColor);
        SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
        int textWidth, textHeight;
        TTF_SizeText(font, "Back", &textWidth, &textHeight);
        SDL_Rect backTextRect = {centerX + (ruleButtonWidth - textWidth) / 2, centerY + ruleButtonHeight + 20 + (backButtonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, backTexture, NULL, &backTextRect);

        SDL_DestroyTexture(titleTexture);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(backTexture);
        SDL_FreeSurface(backSurface);

        TTF_CloseFont(font);
        SDL_DestroyTexture(backgroundTexture);
        SDL_RenderPresent(renderer);
    }
}

void Rule::renderMultilineText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Color& color, int x, int y, int maxWidth, int maxHeight) {
    std::istringstream iss(text);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(iss, line, '\n')) {
        lines.push_back(line);
    }

    for (const auto& line : lines) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, line.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int textWidth, textHeight;
        TTF_SizeText(font, line.c_str(), &textWidth, &textHeight);

        SDL_Rect textRect = {x, y, textWidth, textHeight};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);

        y += textHeight;  // Move to the next line
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}
