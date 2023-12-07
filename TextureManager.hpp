#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "game.hpp"

class TextureManager {
public :
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};

#endif // TEXTUREMANAGER_H
