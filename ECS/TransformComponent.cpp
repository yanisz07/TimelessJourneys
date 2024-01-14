#include "TransformComponent.hpp"
#include "Sword.h"
#include "range_weapon.h"

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
    else if(entity->hasComponent<Range_Weapon>())
    {
        if (!entity->getComponent<Range_Weapon>().is_attacking)
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
