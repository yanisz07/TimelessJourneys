#ifndef INTERACTCOMPONENT_H
#define INTERACTCOMPONENT_H

#include "../game.hpp"
#include "ECS.hpp"
#include "../Vector2D.hpp"
#include "Components.hpp"


    class InteractComponent : public Component
{
private:
    int interactDistance = 170;

public:
    InteractComponent() : interactDistance(170) {}

    void init() override {}
    void update() override {}

    bool PlayerCloseTo(TransformComponent& playerTransform)

    {
        float distance = playerTransform.position.distance(entity->getComponent<TransformComponent>().position);

        return distance <= interactDistance;
    };

};

#endif // INTERACTCOMPONENT_H
