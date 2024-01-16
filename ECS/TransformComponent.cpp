#include "TransformComponent.hpp"
#include "sword.h"
#include "range_weapon.h"

void TransformComponent::update()
{
    if (entity->hasComponent<Sword>() && entity->getComponent<Sword>().is_equiped)
    {
        if (!entity->getComponent<Sword>().is_attacking)
        {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
    }
    else if(entity->hasComponent<Range_Weapon>() && entity->getComponent<Range_Weapon>().is_equiped)
    {
        if (!entity->getComponent<Range_Weapon>().is_attacking)
        {
            std::cout << "player" << std::endl;
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
