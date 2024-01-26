#include "game_over.hpp"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <filesystem>

void renderText(SDL_Renderer* renderer, const std::string &text, int x, int y, int w, int h, TTF_Font* font, SDL_Color color);

void Game_Over::toggleGame0verState(bool &isGameOverOpen) {
    isGameOverOpen = !isGameOverOpen;
}

void Game_Over::renderGameOver(SDL_Renderer* renderer, bool isGameOverOpen, const SDL_Point& mousePosition) {
    int buttonWidth = 150;
    int buttonHeight = 40;
    int borderThickness = 2;

    if (isGameOverOpen) {
        int screenWidth, screenHeight;
        SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

        int centerX = (screenWidth - buttonWidth) / 2;
        int centerY = (screenHeight -  5 * buttonHeight - 20) / 2 + 100;

        // Background
        std::string backgroundPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "displayAssets"/"game_over_screen.png").string();
        SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, backgroundPath.c_str());
        SDL_Rect backgroundRect = {0, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Define button rectangles and borders
        SDL_Rect retryButton = {centerX, centerY, buttonWidth, buttonHeight};
        SDL_Rect exitButton = {centerX, centerY + buttonHeight + 20, buttonWidth, buttonHeight};
        SDL_Rect retryButtonBorder = {centerX - borderThickness, centerY - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};
        SDL_Rect exitButtonBorder = {centerX - borderThickness, centerY + buttonHeight + 20 - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};

        // Check hover states
        bool isHoveringRetry = SDL_PointInRect(&mousePosition, &retryButton);
        bool isHoveringExit = SDL_PointInRect(&mousePosition, &exitButton);

        // Text Rendering setup
        std::string fontPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Arial.ttf").string();
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 24);
        SDL_Color textColorHover = {0, 0, 0, 255};  // Black color for text when hovering
        SDL_Color textColorNormal = {255, 255, 255, 255};  // White color for text when not hovering

        // Render Retry Button
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White for border
        SDL_RenderDrawRect(renderer, &retryButtonBorder);
        SDL_SetRenderDrawColor(renderer, isHoveringRetry ? 255 : 0, isHoveringRetry ? 255 : 0, isHoveringRetry ? 255 : 0, 255);  // White when hovering, black otherwise
        SDL_RenderFillRect(renderer, &retryButton);
        renderText(renderer, "Retry", centerX, centerY, buttonWidth, buttonHeight, font, isHoveringRetry ? textColorHover : textColorNormal);

        // Render Exit Button
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White for border
        SDL_RenderDrawRect(renderer, &exitButtonBorder);
        SDL_SetRenderDrawColor(renderer, isHoveringExit ? 255 : 0, isHoveringExit ? 255 : 0, isHoveringExit ? 255 : 0, 255);  // White when hovering, black otherwise
        SDL_RenderFillRect(renderer, &exitButton);
        renderText(renderer, "Exit Game", centerX, centerY + buttonHeight + 20, buttonWidth, buttonHeight, font, isHoveringExit ? textColorHover : textColorNormal);

        // Cleanup resources like textures and surfaces
        SDL_DestroyTexture(backgroundTexture);
        TTF_CloseFont(font);
        SDL_RenderPresent(renderer);
    }
}

// Utility function to render text
void renderText(SDL_Renderer* renderer, const std::string &text, int x, int y, int w, int h, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int textWidth, textHeight;
    TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);
    SDL_Rect textRect = {x + (w - textWidth) / 2, y + (h - textHeight) / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
