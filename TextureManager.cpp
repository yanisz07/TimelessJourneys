#include "TextureManager.hpp"

const char* root = ROOT_DIR;

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    char result[200];

    strcpy(result,root);
    strcat(result,texture);

    SDL_Surface* tempSurface = IMG_Load(result);

    if(tempSurface == NULL)
    {
        return nullptr;
        std::cout << "Error loading image from: " << result <<std::endl;
    }
    else{
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
    }
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_Color col = {0,0,0,255};
    SDL_SetRenderDrawColor(Game::renderer,0,0,0,255);
    if (SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip) == 0) {
        // The rendering was successful
        // Add any additional code you want to execute on success
    } else {
        /*
        // The rendering failed
        // Add error-handling code here
        const char* sdlError = SDL_GetError();
        // Print or handle the SDL error message as needed

        */
     printf("SDL_RenderCopyEx failed: %s\n",SDL_GetError());
    }

}

void TextureManager::Draw_rotation(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle)
{
    if (SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, flip)== 0) {
        // The rendering was successful
        // Add any additional code you want to execute on success
    } else {
        // The rendering failed
        // Add error-handling code here
        const char* sdlError = SDL_GetError();
        // Print or handle the SDL error message as needed
        printf("SDL_RenderCopyEx failed: %s\n", sdlError);
    }
}

