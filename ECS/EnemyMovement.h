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
    int moveTimer = 0;
    int moveInterval = 120;
    float velocityScale = 0.3;
    float radius_of_attack; //maybe change to int later //250
    float radius_of_displacement;
    float radius_of_pursuit;
    float mindist_enemy_player; //60
    bool pursuit;
    Vector2D initial_position;
    const int chaseDistance = 250; //REMOVE LATER // CHANGES IN GAME.CPP AFTERWARDS.
    const int stopDistance = 60; //REMOVE LATER

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

    void update() override
    {   // Note that we only have to update velocities as they will influence the update of the position in the transform component
        float dist_from_player = calculateDistanceToPlayer(); // Distance enemy-player
        float dist_from_initial_position= calculateDistanceFromInitialPos(); //Distance of enemy's actual position- enemy's initial position
        moveTimer++;
        if (moveTimer>=moveInterval){ // If long enough for velocity changes
            moveTimer = 0;
            if (dist_from_player < radius_of_attack) { // If player close enough from the enemy
                if (dist_from_initial_position < radius_of_pursuit){ // and If the enemy is close enough form its initial position to chase the enemy
                    if (dist_from_player > mindist_enemy_player) { // If enemy not too close from the player
                        // Enemy chases the player
                        std::cout << "Chase mode active" << std::endl;

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
                //else{ //It has to come back to its initial_position (Will see how to do that)
                //}
            }
            else
            { // if the player is far enough from the enemy
                // Move randomly around the map
                std::cout << "Passive mode active" << std::endl;
                if (dist_from_initial_position<radius_of_displacement){
                    // Randomly change direction
                    transform->velocity.x = ((rand() % 3) - 1) * velocityScale; // Random number between -1 and 1
                    transform->velocity.y = ((rand() % 3) - 1) * velocityScale; // Random number between -1 and 1
                }
                else{ // If enemy too for away from its initial position it comes back to get closer to it
                    float x_difference=transform->position.x-initial_position.x;
                    float y_difference=transform->position.x-initial_position.y;
                    float coeff_x=(x_difference/abs(x_difference))*(pow(x_difference,2)/(pow(y_difference,2)+pow(x_difference,2)));
                    float coeff_y=(y_difference/abs(y_difference))*(pow(y_difference,2)/(pow(y_difference,2)+pow(x_difference,2)));
                    transform->velocity.x =coeff_x * velocityScale;
                    transform->velocity.y =coeff_y * velocityScale;
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
