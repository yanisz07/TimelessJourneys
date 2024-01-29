#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "ECS.hpp"
#include "../TextureManager.hpp"
#include "SDL.h"
#include "../Vector2D.hpp"
#include "../AssetManager.hpp"

class TileComponent : public Component
{
public:

    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    Vector2D position;
    int scale1;
    int scale2;
    int tsize;

    TileComponent() = default;

    ~TileComponent()
    {}

    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
    {
        this->tsize = tsize;
        texture = Game::assets->GetTexture(id);

        position.x = static_cast<float>(xpos);
        position.y = static_cast<float>(ypos);

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = tsize;

        destRect.x = xpos;
        destRect.y = ypos;
        destRect.w = destRect.h = tsize * tscale;

        scale1 = tscale;
    }

    void setTileScale2 (int scale) {
        scale2 = scale;

    }
    void update() override {
        if (Game::DisplayMap) {
            // Apply scaling for map view
            destRect.w = tsize * scale2;
            destRect.h = tsize * scale2;
            // Calculate the position for the map view
            destRect.x = static_cast<int>(position.x / scale1) * scale2;
            destRect.y = static_cast<int>(position.y / scale1) * scale2;
        } else {
            // Apply scaling for normal view
            destRect.w = tsize * scale1;
            destRect.h = tsize * scale1;
            // Adjust the position based on the camera
            destRect.x = static_cast<int>(position.x) - Game::camera.x;
            destRect.y = static_cast<int>(position.y) - Game::camera.y;
        }
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};

#endif // TILECOMPONENT_H
