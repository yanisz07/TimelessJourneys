#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "ECS.hpp"
#include "string"
#include "SDL.h"
#include "../timer.hpp"
//#include "Components.hpp"
#include "../TextureManager.hpp"
#include "TransformComponent.hpp"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;

    SDL_Texture* tex = nullptr;
    SDL_Rect srcR, destR;

    TransformComponent* transform;

    Timer timer;

    double angle;

    ColliderComponent(std::string t);

    ColliderComponent(std::string t, int h, int w);

    ColliderComponent(std::string t, int xpos, int ypos, int size);

    ColliderComponent(std::string t, int xpos, int ypos, int width, int height);

    ColliderComponent(std::string t, int xpos, int ypos, int width, int height, int timeOut);

    ColliderComponent(std::string t, int xpos, int ypos, int width, int height, int timeOut, double angle);

    void SetAngle(double angle);

    void init() override;

    void update() override;

    void draw() override;

};

#endif // COLLIDERCOMPONENT_H
