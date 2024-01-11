#include "NPCBehavior.h".h"
#include "TransformComponent.hpp"

NPCBehavior::NPCBehavior(float distance_1, TransformComponent *playerTrans)
{
    playerTransform=playerTrans;
    trigger_distance= distance_1;
    bubble_displayed=0;
}

void NPCBehavior::init()
{
    transform = &entity->getComponent<TransformComponent>();
    srand(time(NULL));
}



void NPCBehavior::update()
{

    float d=calculateDistanceToPlayer();
    bool display=(d<trigger_distance);
    if (display) {

        DisplayBubble();

    } else {
        UndisplayBubble();
    }

}

float NPCBehavior::calculateDistanceToPlayer() const
{
    return transform->position.distance(playerTransform->position);
}

void NPCBehavior::DisplayBubble()
{
    bubble_displayed=1;
}

void NPCBehavior::UndisplayBubble()
{
    bubble_displayed=0;
}
