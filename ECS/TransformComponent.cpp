#include "TransformComponent.hpp"
#include "sword.hpp"
#include "range_weapon.hpp"
#include "math.h"

void TransformComponent::update()
{
    if (entity->hasComponent<Sword>() && entity->getComponent<Sword>().is_equiped)
    {
        if (!entity->getComponent<Sword>().is_attacking)
        {
            if (velocity.x != 0 && velocity.y != 0)
            {
                position.x += velocity.x * speed/std::sqrt(2);
                position.y += velocity.y * speed/std::sqrt(2);
            }
            else
            {
                position.x += velocity.x * speed;
                position.y += velocity.y * speed;
            }
        }
    }
    else if(entity->hasComponent<Range_Weapon>() && entity->getComponent<Range_Weapon>().is_equiped)
    {
        if (!entity->getComponent<Range_Weapon>().is_attacking)
        {
            if (velocity.x != 0 && velocity.y != 0)
            {
                position.x += velocity.x * speed/std::sqrt(2);
                position.y += velocity.y * speed/std::sqrt(2);
            }
            else
            {
                position.x += velocity.x * speed;
                position.y += velocity.y * speed;
            }
        }
    }
    else
    {
        if (velocity.x != 0 && velocity.y != 0)
        {
            position.x += velocity.x * speed/std::sqrt(2);
            position.y += velocity.y * speed/std::sqrt(2);
        }
        else
        {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
    }
}
