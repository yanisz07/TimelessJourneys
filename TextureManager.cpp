#include "TextureManager.hpp"

const char* root = ROOT_DIR;

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    char result[120];

    strcpy(result,root);
    strcat(result,texture);

    SDL_Surface* tempSurface = IMG_Load(result);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}
