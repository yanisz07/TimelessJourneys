#include "ProjectileComponent.hpp"
#include "Stats.hpp"

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
        entity->destroy();
    }
    else if(transform->position.x > Game::camera.x + Game::camera.w ||
             transform->position.x < Game::camera.x ||
             transform->position.y > Game::camera.y + Game::camera.h ||
             transform->position.y < Game::camera.y)
    {
        entity->destroy();
    }
}

void ProjectileComponent::DoDamage(Stats &entity1, Stats &entity2)
{
    entity2.SubtractHealth(entity1.get_damage_mult()*damage);
}
