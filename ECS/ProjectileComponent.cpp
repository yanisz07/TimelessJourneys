#include "ProjectileComponent.hpp"
#include "Stats.hpp"

void ProjectileComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
    transform->velocity = velocity;
    transform->speed = speed;
}

void ProjectileComponent::update()
{
    distance += speed*velocity.norm();

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
    entity2.SubtractHealth(damage);
    if(entity1.is_player())
    {
        if (entity2.get_health()<=0)
        {
            entity1.GainExp(entity2.get_experience_worth());
        }
    }
}

void ProjectileComponent::DoDamage(Stats &entity1)
{
    entity1.SubtractHealth(damage);
}
