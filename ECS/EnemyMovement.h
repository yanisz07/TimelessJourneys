#ifndef ENEMYMOVEMENT_H
#define ENEMYMOVEMENT_H

#include "../game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include <stdlib.h>
#include <math.h>

class EnemyMovement : public Component
{
public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    int moveTimer = 0;
    int moveInterval = 120;
    float velocityScale = 0.3;
    const int chaseDistance = 300;
    const int stopDistance = 60;

    EnemyMovement(TransformComponent* playerTrans) : playerTransform(playerTrans) {}

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        srand(time(NULL));
    }

    void update() override
    {
        float distanceToPlayer = calculateDistanceToPlayer();

        if (distanceToPlayer < chaseDistance) {
            if (distanceToPlayer > stopDistance) {
                // Enemy chases the player
                std::cout << "Chase mod active" << std::endl;

                int xDirection = playerTransform->position.x > transform->position.x ? 1 : -1;  //if  player.x_coord > enemy.x_coord : x_direction = 1 else -1
                int yDirection = playerTransform->position.y > transform->position.y ? 1 : -1;  //if  player.y_coord > enemy.y_coord : y_direction = 1 else -1
                transform->velocity.x = xDirection * velocityScale;
                transform->velocity.y = yDirection * velocityScale;
            } else {
                // Stop the enemy to maintain a distance of 50
                transform->velocity.x = 0;
                transform->velocity.y = 0;
            }
        }
        else
        {
            std::cout << "Passive mod active" << std::endl;
            moveTimer++;
            if (moveTimer >= moveInterval)
            {
                // Move randomly
                moveTimer = 0;
                transform->velocity.x = ((rand() % 3) - 1) * velocityScale;
                transform->velocity.y = ((rand() % 3) - 1) * velocityScale;
            }
        }
    }

    float calculateDistanceToPlayer() const
    {
        float dx = playerTransform->position.x - transform->position.x;
        float dy = playerTransform->position.y - transform->position.y;
        return sqrt(dx * dx + dy * dy);
    }
};

#endif // ENEMYMOVEMENT_H
