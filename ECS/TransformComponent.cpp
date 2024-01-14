#include "TransformComponent.hpp"
#include "Sword.h"

void TransformComponent::update()
{
    if (entity->hasComponent<Sword>())
    {
        if (!entity->getComponent<Sword>().is_attacking)
        {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
    }
    else
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
}
