#ifndef ENEMYMOVEMENT_H
#define ENEMYMOVEMENT_H

#include "../game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>

class EnemyMovement : public Component
{
public:
    TransformComponent* transform;
    TransformComponent* playerTransform; // Pointer to player's transform component
    Vector2D spawnPosition;
    int moveTimer = 0;
    int moveInterval = 200;
    float velocityScale = 0.5;


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        spawnPosition = transform->position;
        srand(time(NULL));
    }


    void update() override
    {
        randomMoveWithinBounds();
    }

private:
    bool isPlayerClose(){};

    bool isInBound(Vector2D positionToCheck) {

        float deltaX = abs(positionToCheck.x - spawnPosition.x);
        float deltaY = abs(positionToCheck.y - spawnPosition.y);

        return deltaX <= 40 && deltaY <= 40;
    }
    void randomMoveWithinBounds()
    {
        moveTimer++;
        if (moveTimer >= moveInterval)
        {
            moveTimer = 0;

            // Generate potential new velocity
            Vector2D potentialVelocity = Vector2D(((rand() % 3) - 1) * velocityScale,
                                                  ((rand() % 3) - 1) * velocityScale);

            // Calculate potential new position
            Vector2D potentialPosition = transform->position + potentialVelocity;

            // Check if the potential new position is within bounds
            if (isInBound(potentialPosition))
            {
                transform->velocity = potentialVelocity;
            }

            else {

                transform->velocity = Vector2D(0,0);


            }
        }
    }

};

#endif // ENEMYMOVEMENT_H
