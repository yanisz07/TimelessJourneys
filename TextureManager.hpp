#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "game.hpp"

class TextureManager {
public :
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    static void Draw_rotation(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle);
};

#endif // TEXTUREMANAGER_H
