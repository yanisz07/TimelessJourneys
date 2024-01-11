#ifndef GAME_OVER_H
#define GAME_OVER_H
#include <SDL.h>
#include <filesystem>

extern std::filesystem::path projectDir;

class Game_Over {
public:
    static void toggleGame0verState(bool &isGameOverOpen);
    static void renderGameOver(SDL_Renderer* renderer, bool isGameOverOpen, const SDL_Point& mousePosition);

};
#endif // GAME_OVER_H
