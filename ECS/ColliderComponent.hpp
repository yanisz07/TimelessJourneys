#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "string"
#include "SDL.h"
#include "Components.hpp"
#include "../TextureManager.hpp"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;

    SDL_Texture* tex;
    SDL_Rect srcR, destR;

    TransformComponent* transform;

    ColliderComponent(std::string t)
    {
        tag = t;
    }

    ColliderComponent(std::string t, int h, int w)
    {
        tag = t;
        collider.h = h;
        collider.w = w;
    }

    ColliderComponent(std::string t, int xpos, int ypos, int size)
    {
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.h = collider.w = size;
    }

    void init() override
    {
        if(!(entity->hasComponent<TransformComponent>()))
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();

        tex = TextureManager::LoadTexture("/assets/ColTex.png");
        srcR = {0, 0, 32, 32 };
        destR = { collider.x, collider.y, collider.w, collider.h };

    }

    void update() override
    {
        if(tag != "terrain")
        {
            if (tag == "player" || tag == "enemy")
            {
                // fix collider for player and enemy
                collider.x = static_cast<int>(transform->position.x)+35;
                collider.y = static_cast<int>(transform->position.y)+80;
                collider.w = transform->width * transform->scale-78;
                collider.h = transform->height * transform->scale-80;
            }
            else
            {
                collider.x = static_cast<int>(transform->position.x);
                collider.y = static_cast<int>(transform->position.y);
                collider.w = transform->width * transform->scale;
                collider.h = transform->height * transform->scale;
            }
        }

        destR.x = collider.x - Game::camera.x;
        destR.y = collider.y - Game::camera.y;
    }

    void draw() override
    {
        /*if (tag=="player")
        {
            std::cout << "collider drawn" << std::endl;
        }*/
        TextureManager::Draw(tex,srcR,destR,SDL_FLIP_NONE);
    }

};

#endif // COLLIDERCOMPONENT_H
