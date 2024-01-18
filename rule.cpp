#include "rule.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <filesystem>

//brief
//FCHGDFB
void Rule::toggleRuleState(bool &isRuleOpen) {
    isRuleOpen = !isRuleOpen;
}

void Rule::renderRule(SDL_Renderer* renderer, bool isRuleOpen, const SDL_Point& mousePosition) {
    int buttonWidth = 150;
    int buttonHeight = 40;
    int titleButtonWidth = 300;
    int titleButtonHeight = 60;
    int borderThickness = 2;

    if (isRuleOpen) {
        int screenWidth, screenHeight;
        SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

        int titleCenterX = (screenWidth - titleButtonWidth) / 2;
        int titleCenterY = 50;
        int centerX = (screenWidth - buttonWidth) / 2;
        int centerY = (screenHeight - 2 * buttonHeight - 20) / 2 + 100;

        // Background
        std::string backgroundPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "background_sample1.png").string();
        SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, backgroundPath.c_str());
        SDL_Rect backgroundRect = {0, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


        SDL_Rect titleButtonBorder = {titleCenterX - borderThickness, titleCenterY - borderThickness, titleButtonWidth + borderThickness * 2, titleButtonHeight + borderThickness * 2};
        SDL_Rect ruleButtonBorder = {centerX - borderThickness, centerY - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};
        SDL_Rect backButtonBorder = {centerX - borderThickness, centerY + buttonHeight + 20 - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};


        SDL_RenderDrawRect(renderer, &titleButtonBorder);
        SDL_RenderDrawRect(renderer, &ruleButtonBorder);
        SDL_RenderDrawRect(renderer, &backButtonBorder);



        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); //vanilla color

        // Buttons
        SDL_Rect titleButton = {titleCenterX, titleCenterY, titleButtonWidth, titleButtonHeight};
        SDL_Rect ruleButton = {centerX, centerY, buttonWidth, buttonHeight};
        SDL_Rect backsButton = {centerX, centerY + buttonHeight + 20, buttonWidth, buttonHeight};

        //hover effect
        bool isHoveringbacks = SDL_PointInRect(&mousePosition, &backsButton);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for borders
        SDL_RenderDrawRect(renderer, &titleButton);
        SDL_RenderDrawRect(renderer, &ruleButton);
        SDL_RenderDrawRect(renderer, &backsButton);

        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Beige color for buttons
        SDL_RenderFillRect(renderer, &titleButton);



        // hover effect for backs button
        if (isHoveringbacks) {
            SDL_SetRenderDrawColor(renderer, 253, 254, 224, 255); // Lighter color for hover
        } else {
            SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Original color
        }
        SDL_RenderFillRect(renderer, &backsButton);




        // Text Rendering
        std::string fontPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Arial.ttf").string();
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 24);
        SDL_Color textColor = {0, 0, 0, 255};

        // Render Title Text
        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Timeless JourneysDan", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        int titleTextWidth, titleTextHeight;
        TTF_SizeText(font, "Timeless JourneysDan", &titleTextWidth, &titleTextHeight);
        SDL_Rect titleTextRect = {titleCenterX + (titleButtonWidth - titleTextWidth) / 2, titleCenterY + (titleButtonHeight - titleTextHeight) / 2, titleTextWidth, titleTextHeight};
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleTextRect);

        // Render rule Text
        SDL_Surface* ruleSurface = TTF_RenderText_Solid(font, "Start", textColor);
        SDL_Texture* ruleTexture = SDL_CreateTextureFromSurface(renderer, ruleSurface);
        int textWidth, textHeight;
        TTF_SizeText(font, "Start", &textWidth, &textHeight);
        SDL_Rect ruleTextRect = {centerX + (buttonWidth - textWidth) / 2, centerY + (buttonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, ruleTexture, NULL, &ruleTextRect);

        // Render back Text
        SDL_Surface* backsSurface = TTF_RenderText_Solid(font, "backs", textColor);
        SDL_Texture* backsTexture = SDL_CreateTextureFromSurface(renderer, backsSurface);
        TTF_SizeText(font, "backs", &textWidth, &textHeight);
        SDL_Rect backsTextRect = {centerX + (buttonWidth - textWidth) / 2, centerY + buttonHeight + 20 + (buttonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, backsTexture, NULL, &backsTextRect);







        SDL_DestroyTexture(titleTexture);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(ruleTexture);
        SDL_FreeSurface(ruleSurface);
        SDL_DestroyTexture(backsTexture);
        SDL_FreeSurface(backsSurface);

        TTF_CloseFont(font);
        SDL_DestroyTexture(backgroundTexture);
        SDL_RenderPresent(renderer);
    }
}
