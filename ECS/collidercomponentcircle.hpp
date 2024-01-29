#ifndef COLLIDERCOMPONENTCIRCLE_H
#define COLLIDERCOMPONENTCIRCLE_H

#include "ECS.hpp"
#include "string"
#include "SDL.h"
#include "../timer.hpp"
#include "../TextureManager.hpp"
#include "TransformComponent.hpp"

class ColliderComponentCircle : public Component
{
public:
    std::string tag;

    SDL_Texture* tex;
    SDL_Rect srcR, destR;

    TransformComponent* transform;

    Timer timer;

    Vector2D center;

    double radius;

    ColliderComponentCircle(std::string t,double r);

    void init() override;

    void update() override;

    void draw() override;

    ~ColliderComponentCircle() override;
};

#endif // COLLIDERCOMPONENTCIRCLE_H
