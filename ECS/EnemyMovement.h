#ifndef ENEMYMOVEMENT_H
#define ENEMYMOVEMENT_H

#include "ECS.hpp"
#include "Components.hpp"
#include <stdlib.h>
#include <math.h>

class EnemyMovement : public Component
{
public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    int collisionCooldown = 0;
    const int collisionCooldownMax = 10;
    int moveTimer = 0;
    int moveInterval = 120;
    float velocityScale = 0.3;
    float radius_of_attack;
    float radius_of_displacement;
    float radius_of_pursuit;
    float mindist_enemy_player;
    bool pursuit;
    Vector2D initial_position;

    EnemyMovement(float radius_1, float radius_2, float radius_3, float distance_1, TransformComponent* playerTrans){
        playerTransform=playerTrans;
        radius_of_attack=radius_1;
        radius_of_displacement=radius_2;
        radius_of_pursuit=radius_3;
        mindist_enemy_player= distance_1;
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        initial_position=transform->position;
        srand(time(NULL));
    }

    void onCollision(){
        transform->velocity.x *= -1.5;
        transform->velocity.y *= -1.5;
        transform->position.x += transform->velocity.x * 5;
        transform->position.y += transform->velocity.y * 5;
        collisionCooldown = collisionCooldownMax;
    }

    void returnToInitialPosition() { // PAY ATTETION WITH VECTOR2D OPERATIONS.
        Vector2D direction;
        direction.x=initial_position.x - transform->position.x;
        direction.y=initial_position.y - transform->position.y;
        float distance = direction.magnitude();

        if (distance < 1.0f) { // Avoid division by zero
            transform->velocity.x = 0;
            transform->velocity.y = 0;
        } else {
            // Normalize direction and scale with velocityScale
            direction.x /= distance;
            direction.y /= distance;
            transform->velocity.x = direction.x * velocityScale;
            transform->velocity.y = direction.y * velocityScale;
        }
    }

    void update() override
    {   // Note that we only have to update velocities as they will influence the update of the position in the transform component 
        if (collisionCooldown > 0) {
            collisionCooldown--;
            return; // Skip collision checks
        }
        moveTimer++;
        if (moveTimer>=moveInterval){ // If long enough for velocity changes
            moveTimer = 0;
            float dist_from_player = calculateDistanceToPlayer(); // Distance enemy-player
            float dist_from_initial_position= calculateDistanceFromInitialPos(); //Distance of enemy's actual position- enemy's initial position
            if (dist_from_player < radius_of_attack) { // If player close enough from the enemy
                //std::cout << "In radius of attack" << std::endl;
                if (dist_from_initial_position < radius_of_pursuit){ // and If the enemy is close enough form its initial position to chase the enemy
                    //std::cout << "In radius of pursuit" << std::endl;
                    if (dist_from_player > mindist_enemy_player) { // If enemy not too close from the player
                        // Enemy chases the player
                        //std::cout << "Chase mode active" << std::endl;
                        int xDirection = playerTransform->position.x > transform->position.x ? 1 : -1;  //if  player.x_coord > enemy.x_coord : x_direction = 1 else -1
                        int yDirection = playerTransform->position.y > transform->position.y ? 1 : -1;  //if  player.y_coord > enemy.y_coord : y_direction = 1 else -1
                        transform->velocity.x = xDirection * velocityScale;
                        transform->velocity.y = yDirection * velocityScale;
                    }
                    else { // Otherwise, if enemy too close from player, it stops until the player is far enough
                        transform->velocity.x = 0;
                        transform->velocity.y = 0;
                    }
                }
                else{ //It has to come back to its initial_position (Will see how to do that)
                    //std::cout << "Outside radius of pursuit" << std::endl;
                    returnToInitialPosition();
                }
            }
            else{ // if the player is far enough from the enemy
                // Move randomly around the map
                //std::cout << "Passive mode active" << std::endl;
                if (dist_from_initial_position<radius_of_displacement){
                    // Randomly change direction
                    //std::cout << "randomly moves in zone" << std::endl;
                    transform->velocity.x = ((rand() % 3) - 1) * velocityScale; // Random number between -1 and 1
                    transform->velocity.y = ((rand() % 3) - 1) * velocityScale; // Random number between -1 and 1
                }
                else{ // If enemy too for away from its initial position it comes back to get closer to it
                    //std::cout << "tries to come back to its initial position" << std::endl;
                    returnToInitialPosition();
                }
            }
        }
    }

    float calculateDistanceToPlayer() const
    {
        return transform->position.distance(playerTransform->position);
    }

    float calculateDistanceFromInitialPos() const
    {
        return transform->position.distance(initial_position);
    }
};

#endif // ENEMYMOVEMENT_H
