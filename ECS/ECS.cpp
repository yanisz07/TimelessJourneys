#include "ECS.hpp"

void Entity::addGroup(Group mGroup)
{
    groupBitset[mGroup] = true;
    manager.AddToGroup(this, mGroup);
}

void Component::setPriority(int priorityLevel)
{
    priority = priorityLevel;
    entity->orderComponents();
}

std::string Component::getEntityType()
{
    return entity->type;
}
