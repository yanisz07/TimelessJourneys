#include "collidercomponentcircle.h"

ColliderComponentCircle::ColliderComponentCircle(std::string t, double r)
{
    tag = t;
    radius = r;
}

void ColliderComponentCircle::init()
{
    tex = TextureManager::LoadTexture("/assets/32x32_circle.png");
    if (entity->hasComponent<TransformComponent>())
    {
        transform = &entity->getComponent<TransformComponent>();
        center.x = transform->position.x + radius;
        center.y = transform->position.y + radius;
    }
    srcR = {0,0,32,32};
    destR = {static_cast<int>(transform->position.x),static_cast<int>(transform->position.y),static_cast<int>(2*radius),static_cast<int>(2*radius)};
}

void ColliderComponentCircle::update()
{
    center.x = transform->position.x + radius;
    center.y = transform->position.y + radius;
    destR.x = static_cast<int>(transform->position.x) - Game::camera.x;
    destR.y = static_cast<int>(transform->position.y) - Game::camera.y;
    destR.w = static_cast<int>(2*radius);
    destR.h = static_cast<int>(2*radius);
}

void ColliderComponentCircle::draw()
{
    TextureManager::Draw(tex,srcR,destR,SDL_FLIP_NONE);
}
