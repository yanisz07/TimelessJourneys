#ifndef SETTING_H
#define SETTING_H
#include <SDL.h>
#include <filesystem>

extern std::filesystem::path projectDir;

class Setting {
public:
    static void toggleSettingState(bool &isSettingsOpen);
    static void renderSetting(SDL_Renderer* renderer, bool isSettingsOpen,const SDL_Point& mousePosition);
};


#endif // SETTING_H



