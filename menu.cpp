#include "menu.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <filesystem>


void Menu::toggleMenuState(bool &isMenuOpen) {
    isMenuOpen = !isMenuOpen;
}

void Menu::renderMenu(SDL_Renderer* renderer, bool isMenuOpen, const SDL_Point& mousePosition) {
    int buttonWidth = 150;
    int buttonHeight = 40;
    int titleButtonWidth = 300;
    int titleButtonHeight = 60;
    int borderThickness = 2;

    if (isMenuOpen) {
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
        SDL_Rect playButtonBorder = {centerX - borderThickness, centerY - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};
        SDL_Rect settingsButtonBorder = {centerX - borderThickness, centerY + buttonHeight + 20 - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};


        SDL_RenderDrawRect(renderer, &titleButtonBorder);
        SDL_RenderDrawRect(renderer, &playButtonBorder);
        SDL_RenderDrawRect(renderer, &settingsButtonBorder);


        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); //vanilla color

        // Buttons
        SDL_Rect titleButton = {titleCenterX, titleCenterY, titleButtonWidth, titleButtonHeight};
        SDL_Rect playButton = {centerX, centerY, buttonWidth, buttonHeight};
        SDL_Rect settingsButton = {centerX, centerY + buttonHeight + 20, buttonWidth, buttonHeight};

        //hover effect
        bool isHoveringPlay = SDL_PointInRect(&mousePosition, &playButton);
        bool isHoveringSettings = SDL_PointInRect(&mousePosition, &settingsButton);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for borders
        SDL_RenderDrawRect(renderer, &titleButton);
        SDL_RenderDrawRect(renderer, &playButton);
        SDL_RenderDrawRect(renderer, &settingsButton);

        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Beige color for buttons
        SDL_RenderFillRect(renderer, &titleButton);

        // hover effect for Play button
        if (isHoveringPlay) {
            SDL_SetRenderDrawColor(renderer, 253, 254, 224, 255); // Lighter color for hover
            SDL_RenderFillRect(renderer, &playButton);
        } else {
            SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Original color
            SDL_RenderFillRect(renderer, &playButton);
        }

        // hover effect for Settings button
        if (isHoveringSettings) {
            SDL_SetRenderDrawColor(renderer, 253, 254, 224, 255); // Lighter color for hover
            SDL_RenderFillRect(renderer, &settingsButton);
        } else {
            SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Original color
            SDL_RenderFillRect(renderer, &settingsButton);
        }

        SDL_RenderFillRect(renderer, &titleButton);
        SDL_RenderFillRect(renderer, &playButton);
        SDL_RenderFillRect(renderer, &settingsButton);

        // Text Rendering
        std::string fontPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Arial.ttf").string();
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 24);
        SDL_Color textColor = {0, 0, 0, 255};

        // Render Title Text
        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Timeless Journeys", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        int titleTextWidth, titleTextHeight;
        TTF_SizeText(font, "Timeless Journeys", &titleTextWidth, &titleTextHeight);
        SDL_Rect titleTextRect = {titleCenterX + (titleButtonWidth - titleTextWidth) / 2, titleCenterY + (titleButtonHeight - titleTextHeight) / 2, titleTextWidth, titleTextHeight};
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleTextRect);

        // Render Play Text
        SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start", textColor);
        SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        int textWidth, textHeight;
        TTF_SizeText(font, "Start", &textWidth, &textHeight);
        SDL_Rect startTextRect = {centerX + (buttonWidth - textWidth) / 2, centerY + (buttonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, startTexture, NULL, &startTextRect);

        // Render Setting Text
        SDL_Surface* settingsSurface = TTF_RenderText_Solid(font, "Settings", textColor);
        SDL_Texture* settingsTexture = SDL_CreateTextureFromSurface(renderer, settingsSurface);
        TTF_SizeText(font, "Settings", &textWidth, &textHeight);
        SDL_Rect settingsTextRect = {centerX + (buttonWidth - textWidth) / 2, centerY + buttonHeight + 20 + (buttonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, settingsTexture, NULL, &settingsTextRect);





        SDL_DestroyTexture(titleTexture);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(startTexture);
        SDL_FreeSurface(startSurface);
        SDL_DestroyTexture(settingsTexture);
        SDL_FreeSurface(settingsSurface);
        TTF_CloseFont(font);
        SDL_DestroyTexture(backgroundTexture);
        SDL_RenderPresent(renderer);
    }
}
