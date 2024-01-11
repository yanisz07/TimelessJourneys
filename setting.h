#ifndef SETTING_H
#define SETTING_H
#include <SDL.h>
#include <filesystem>

extern std::filesystem::path projectDir;

class Setting {
public:
    static void toggleSettingState(bool &isSettingsOpen);
    static void renderSetting(SDL_Renderer* renderer, bool isSettingsOpen, const SDL_Point& mousePosition, bool isFullscreen, bool isMusic);
    static void handleSliderEvent(const SDL_Point& mousePosition);

private:
    static int currentVolume;  // Range 0-100
    static SDL_Rect volumeSliderBar;  // Rect for the slider bar
    static SDL_Rect volumeSliderHandle;  // Rect for the slider handle
};

#endif // SETTING_H
