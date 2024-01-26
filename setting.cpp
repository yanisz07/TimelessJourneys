#include "setting.hpp"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <filesystem>
#include <SDL_mixer.h>


int Setting::currentVolume = 50; // Starting volume
SDL_Rect Setting::volumeSliderBar = {0, 0, 250, 40};
SDL_Rect Setting::volumeSliderHandle = {0 + (currentVolume * 2), 100, 10, 40};

void Setting::toggleSettingState(bool &isSettingsOpen) {
    isSettingsOpen = !isSettingsOpen;
}

void Setting::renderSetting(SDL_Renderer* renderer, bool isSettingsOpen, const SDL_Point& mousePosition,bool isFullscreen, bool isMusic) {
    int buttonWidth = 250;
    int buttonHeight = 40;
    int titleButtonWidth = 300;
    int titleButtonHeight = 60;
    int borderThickness = 2;

    if (isSettingsOpen) {
        int screenWidth, screenHeight;
        SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

        int titleCenterX = (screenWidth - titleButtonWidth) / 2;
        int titleCenterY = 50;
        int centerX = (screenWidth - buttonWidth) / 2;
        int centerY = (screenHeight - 2 * buttonHeight - 20) / 2 + 100;


        // Background
        std::string backgroundPath = (projectDir / ".." / "TimelessJourneys" / "assets" / "displayAssets"/"background_sample1.png").string();
        SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, backgroundPath.c_str());
        SDL_Rect backgroundRect = {0, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


        SDL_Rect titleButtonBorder = {titleCenterX - borderThickness, titleCenterY - borderThickness, titleButtonWidth + borderThickness * 2, titleButtonHeight + borderThickness * 2};
        SDL_Rect screendimButtonBorder = {centerX - borderThickness, centerY  - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};
        SDL_Rect MusicButtonBorder = {centerX - borderThickness, centerY + buttonHeight + 20 - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};
        SDL_Rect backButtonBorder = {centerX - borderThickness, centerY +3*buttonHeight + 60 - borderThickness, buttonWidth + borderThickness * 2, buttonHeight + borderThickness * 2};

        int volumeSliderX = (screenWidth - volumeSliderBar.w) / 2;
        int volumeSliderY = centerY + 2*buttonHeight + 40;

        volumeSliderBar.x = volumeSliderX;
        volumeSliderBar.y = volumeSliderY;
        volumeSliderHandle.x = volumeSliderBar.x + (currentVolume * (volumeSliderBar.w - volumeSliderHandle.w) / 100);
        volumeSliderHandle.y = volumeSliderY;

        SDL_RenderDrawRect(renderer, &titleButtonBorder);
        SDL_RenderDrawRect(renderer, &backButtonBorder);
        SDL_RenderDrawRect(renderer, &MusicButtonBorder);
        SDL_RenderDrawRect(renderer, &screendimButtonBorder);



        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); //vanilla color

        // Buttons
        SDL_Rect titleButton = {titleCenterX, titleCenterY, titleButtonWidth, titleButtonHeight};
        SDL_Rect ScreenDimButton = {centerX, centerY, buttonWidth, buttonHeight};
        SDL_Rect MusicButton = {centerX, centerY + buttonHeight + 20, buttonWidth, buttonHeight};
        SDL_Rect backButton = {centerX, centerY + 3*buttonHeight + 60, buttonWidth, buttonHeight};

        //hover effect
        bool isHoveringScreenDim = SDL_PointInRect(&mousePosition, &ScreenDimButton);
        bool isHoveringMusic = SDL_PointInRect(&mousePosition, &MusicButton);
        bool isHoveringback = SDL_PointInRect(&mousePosition, &backButton);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for borders
        SDL_RenderDrawRect(renderer, &titleButton);
        SDL_RenderDrawRect(renderer, &backButton);
        SDL_RenderDrawRect(renderer, &ScreenDimButton);
        SDL_RenderDrawRect(renderer, &MusicButton);

        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Beige color for buttons
        SDL_RenderFillRect(renderer, &titleButton);



        // hover effect for Screen Dimension button
        if (isHoveringScreenDim) {
            SDL_SetRenderDrawColor(renderer, 253, 254, 224, 255); // Lighter color for hover
        } else {
            SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Original color
        }
        SDL_RenderFillRect(renderer, &ScreenDimButton);

        // hover effect for Music button
        if (isHoveringMusic) {
            SDL_SetRenderDrawColor(renderer, 253, 254, 224, 255); // Lighter color for hover
        } else {
            SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Original color
        }
        SDL_RenderFillRect(renderer, &MusicButton);

        // hover effect for Back button
        if (isHoveringback) {
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
        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Settings", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        int titleTextWidth, titleTextHeight;
        TTF_SizeText(font, "Settings", &titleTextWidth, &titleTextHeight);
        SDL_Rect titleTextRect = {titleCenterX + (titleButtonWidth - titleTextWidth) / 2, titleCenterY + (titleButtonHeight - titleTextHeight) / 2, titleTextWidth, titleTextHeight};
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleTextRect);

        // Render Full Screen Text
        // adjusts text based on state of full screen function
        int textWidth, textHeight;

        std::string fullScreenText = isFullscreen ? "Full Screen: On" : "Full Screen: Off";
        SDL_Surface* ScreenDimSurface = TTF_RenderText_Solid(font, fullScreenText.c_str(), textColor);
        SDL_Texture* ScreenDimTexture = SDL_CreateTextureFromSurface(renderer, ScreenDimSurface);
        TTF_SizeText(font, fullScreenText.c_str(), &textWidth, &textHeight);
        SDL_Rect ScreenDimTextRect = {centerX + (buttonWidth - textWidth) / 2, centerY + (buttonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, ScreenDimTexture, NULL, &ScreenDimTextRect);

        // Render Music Text
        // adjusts text based on state of music function

        std::string MusicText = isMusic ? "Music: On" : "Music: Off";
        SDL_Surface* MusicSurface = TTF_RenderText_Solid(font, MusicText.c_str(), textColor);
        SDL_Texture*MusicTexture = SDL_CreateTextureFromSurface(renderer, MusicSurface);
        TTF_SizeText(font, MusicText.c_str(), &textWidth, &textHeight);
        SDL_Rect MusicTextRect = {centerX + (buttonWidth - textWidth) / 2, centerY + buttonHeight + 20 + (buttonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, MusicTexture, NULL, &MusicTextRect);


        // Render slider

        SDL_SetRenderDrawColor(renderer, 253, 244, 214, 255); // Beige color for slider background
        SDL_RenderFillRect(renderer, &volumeSliderBar);

        SDL_SetRenderDrawColor(renderer, 0, 122, 204, 255); // Blue color for handle
        SDL_RenderFillRect(renderer, &volumeSliderHandle);



        TTF_CloseFont(font);
        font = TTF_OpenFont(fontPath.c_str(), 18);
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC); // font for slider in italics

        SDL_Surface* sliderTextSurface = TTF_RenderText_Solid(font, "Click to adjust music volume", textColor);
        SDL_Texture* sliderTextTexture = SDL_CreateTextureFromSurface(renderer, sliderTextSurface);
        int sliderTextWidth, sliderTextHeight;
        TTF_SizeText(font, "Click to adjust music volume", &sliderTextWidth, &sliderTextHeight);
        SDL_Rect sliderTextRect = {
            volumeSliderBar.x + (volumeSliderBar.w - sliderTextWidth) / 2,
            volumeSliderBar.y + volumeSliderBar.h - 35, // Adjust Y position as needed
            sliderTextWidth,
            sliderTextHeight
        };
        SDL_RenderCopy(renderer, sliderTextTexture, NULL, &sliderTextRect);

        TTF_CloseFont(font);
        font = TTF_OpenFont(fontPath.c_str(), 24);
        TTF_SetFontStyle(font, TTF_STYLE_NORMAL); // reset to not italics


        // Render Back Text
        SDL_Surface* backSurface = TTF_RenderText_Solid(font, "back", textColor);
        SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
        TTF_SizeText(font, "back", &textWidth, &textHeight);
        SDL_Rect backTextRect = {centerX + (buttonWidth - textWidth) / 2, centerY + 3*buttonHeight + 60 + (buttonHeight - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, backTexture, NULL, &backTextRect);




        SDL_DestroyTexture(titleTexture);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(backTexture);
        SDL_FreeSurface(backSurface);
        SDL_DestroyTexture(MusicTexture);
        SDL_FreeSurface(MusicSurface);
        TTF_CloseFont(font);
        SDL_DestroyTexture(backgroundTexture);
        SDL_RenderPresent(renderer);
    }
}

void Setting::handleSliderEvent(const SDL_Point& mousePosition) {
    if (mousePosition.x >= volumeSliderBar.x && mousePosition.x <= volumeSliderBar.x + volumeSliderBar.w &&
        mousePosition.y >= volumeSliderBar.y && mousePosition.y <= volumeSliderBar.y + volumeSliderBar.h) {
        currentVolume = (mousePosition.x - volumeSliderBar.x) * 100 / volumeSliderBar.w;
        if (currentVolume > 100) currentVolume = 100;
        if (currentVolume < 0) currentVolume = 0;

        volumeSliderHandle.x = volumeSliderBar.x + (currentVolume * (volumeSliderBar.w - volumeSliderHandle.w) / 100);

        Mix_VolumeMusic(currentVolume * 128 / 100);

    }
}


void Setting::volume_onoff(bool isMusic) {
    if (isMusic){
        currentVolume = 100;
        Mix_VolumeMusic(currentVolume * 128 / 100);

    }
    else if (!isMusic){
        currentVolume = 0;
        Mix_VolumeMusic(currentVolume * 128 / 100);

    }
}
