#include "ColliderComponent.hpp"

ColliderComponent::ColliderComponent(std::string t)
{
    tag = t;
    angle = 0;
}

ColliderComponent::ColliderComponent(std::string t, int h, int w)
{
    tag = t;
    collider.h = h;
    collider.w = w;
    angle = 0;
}

ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos, int size)
{
    tag = t;
    collider.x = xpos;
    collider.y = ypos;
    collider.h = collider.w = size;
    angle = 0;
}

ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos, int width, int height)
{
    tag = t;
    collider.x = xpos;
    collider.y = ypos;
    collider.h = height;
    collider.w = width;
    angle = 0;
}

ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos, int width, int height, int timeOut)
{
    tag = t;
    collider.x = xpos;
    collider.y = ypos;
    collider.h = height;
    collider.w = width;
    timer.setTimeOut(timeOut);
    angle = 0;
}

ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos, int width, int height, int timeOut, double angle)
{
    tag = t;
    collider.x = xpos;
    collider.y = ypos;
    collider.h = height;
    collider.w = width;
    timer.setTimeOut(timeOut);
    this->angle=angle;
}

void ColliderComponent::SetAngle(double angle)
{
    this->angle = angle;
}

void ColliderComponent::init()
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

void ColliderComponent::update()
{
    if(tag != "terrain" && tag != "player_attack")
    {
        if (tag=="player")
        {
            // fix collider for player
            collider.x = static_cast<int>(transform->position.x+18*transform->scale);
            collider.y = static_cast<int>(transform->position.y+13*transform->scale);
            collider.w = 13 * transform->scale;
            collider.h = 19 * transform->scale;
        }
        else if (tag == "enemy")
        {
            // fix collider for enemy
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

    if(tag == "player_attack")
    {
        if (timer.timedOut())
        {
            entity->destroy();
        }
    }

    destR.x = collider.x - Game::camera.x;
    destR.y = collider.y - Game::camera.y;
}

void ColliderComponent::draw()
{
    if (tag!= "player" && tag!= "enemy")
    {
        if (angle == 0)
        {
            TextureManager::Draw(tex,srcR,destR,SDL_FLIP_NONE);
        }
        else
        {
            TextureManager::Draw_rotation(tex,srcR,destR,SDL_FLIP_NONE,angle);
        }
    }
}