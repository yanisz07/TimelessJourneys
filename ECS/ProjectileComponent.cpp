#include "ProjectileComponent.hpp"
#include "Stats.hpp"
#include "armor.hpp""

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
        std::cout << "Projectile out of range" << std::endl;
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

void ProjectileComponent::DoDamage(Stats &entity1, Armor &entity1armor)
{
    entity1.SubtractHealth(static_cast<int>(damage/entity1armor.get_armor()));
}
