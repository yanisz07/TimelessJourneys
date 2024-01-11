#ifndef NPCBEHAVIOR_H
#define NPCBEHAVIOR_H


#include "ECS.hpp"
#include <stdlib.h>
#include <math.h>
#include "../Vector2D.hpp"


//forward decleration
class TransformComponent;
//end;

class NPCBehavior : public Component
{
public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    float trigger_distance;
    bool bubble_displayed;

    NPCBehavior(float distance_1, TransformComponent* playerTrans);

    void init() override;

    void update() override;

    float calculateDistanceToPlayer() const;

    void DisplayBubble();
    void UndisplayBubble();

};







#endif // NPCBEHAVIOR_H
