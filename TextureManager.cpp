#include "TextureManager.h"

const char* root = ROOT_DIR;

SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* ren)
{
    char result[120];

    strcpy(result,root);
    strcat(result,texture);

    SDL_Surface* tempSurface = IMG_Load(result);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}
