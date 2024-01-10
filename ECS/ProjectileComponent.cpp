#include "ProjectileComponent.hpp"

void ProjectileComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
    transform->velocity = velocity;
}

void ProjectileComponent::update()
{
    distance += speed;

    if(distance > range)
    {
        //std::cout << "out of range" << std::endl;
        entity->destroy();
    }
    else if(transform->position.x > Game::camera.x + Game::camera.w ||
             transform->position.x < Game::camera.x ||
             transform->position.y > Game::camera.y + Game::camera.h ||
             transform->position.y < Game::camera.y)
    {
        //std::cout << "out of bounds" << std::endl;
        entity->destroy();
    }
}
