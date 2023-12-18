#include "TextureManager.h"

const char* root = ROOT_DIR;

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    char result[200];

    strcpy(result,root);
    strcat(result,texture);

    SDL_Surface* tempSurface = IMG_Load(result);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip); //NULL -> no rotation for now
}
