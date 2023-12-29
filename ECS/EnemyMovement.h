#ifndef ENEMYMOVEMENT_H
#define ENEMYMOVEMENT_H

#include "../game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include <stdlib.h>
#include <time.h>

class EnemyMovement : public Component
{
public:
    TransformComponent* transform;
    SpriteComponent* sprite;
    int moveTimer = 0;
    int moveInterval = 120; // Number of frames to wait before changing direction
    float velocityScale = 0.5; // Scale factor for velocity

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        srand(time(NULL)); // Seed the random number generator
    }

    void update() override
    {
        moveTimer++;
        if (moveTimer >= moveInterval)
        {
            moveTimer = 0;
            // Randomly change direction with scaled velocity
            transform->velocity.x = ((rand() % 3) - 1) * velocityScale;
            transform->velocity.y = ((rand() % 3) - 1) * velocityScale;           
        }
    }
};

#endif // ENEMYMOVEMENT_H
