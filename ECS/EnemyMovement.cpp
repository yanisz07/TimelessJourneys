#include "EnemyMovement.h"
#include "TransformComponent.hpp"
#include <iostream>
#include <vector>
#include <tuple>
//#include <cmath>
#include "Stats.hpp"


EnemyMovement::EnemyMovement(int enemy_type, float radius_1, float radius_2, float radius_3, float distance_1, TransformComponent *playerTrans, Stats *playerstats, Stats* e_stats)
{
    enemyType = enemy_type;
    playerTransform=playerTrans;
    playerStats = playerstats;
    stats = e_stats;
    radius_of_attack=radius_1;
    radius_of_displacement=radius_2;
    radius_of_pursuit=radius_3;
    mindist_enemy_player= distance_1;
}

void EnemyMovement::init()
{
    transform = &entity->getComponent<TransformComponent>();
    initial_position=transform->position;
    srand(time(NULL));
}

void EnemyMovement::onCollision(SDL_Rect collider_rect) {

    Vector2D enemyCenter(transform->position.x + transform->width / 2, transform->position.y + transform->height / 2);
    Vector2D collidertopleft(collider_rect.x, collider_rect.y + collider_rect.h);
    Vector2D collidertopright(collider_rect.x + collider_rect.w, collider_rect.y + collider_rect.h);
    Vector2D colliderbottomleft(collider_rect.x, collider_rect.y);
    Vector2D colliderbottomright(collider_rect.x + collider_rect.w, collider_rect.y);

    Vector2D rightanticlock = collidertopright - colliderbottomright;
    Vector2D topanticlock = collidertopleft -  collidertopright;
    Vector2D leftanticlock = colliderbottomleft - collidertopleft;
    Vector2D bottomanticlock = colliderbottomright - colliderbottomleft;

    std::vector<std::tuple<Vector2D,Vector2D,Vector2D>> list_of_tuples;

    list_of_tuples.push_back(std::make_tuple(rightanticlock,colliderbottomright,collidertopright));
    list_of_tuples.push_back(std::make_tuple(topanticlock,collidertopright,collidertopleft));
    list_of_tuples.push_back(std::make_tuple(leftanticlock,collidertopleft,colliderbottomleft));
    list_of_tuples.push_back(std::make_tuple(bottomanticlock,colliderbottomleft,colliderbottomright));

    int mini_idx = 0;
    double mini_dist_wall = enemyCenter.distance(std::get<1>(list_of_tuples[0]))+enemyCenter.distance(std::get<2>(list_of_tuples[0]));

    for (size_t i = 1; i < list_of_tuples.size(); i++) {
        double candidate = enemyCenter.distance(std::get<1>(list_of_tuples[i]))+enemyCenter.distance(std::get<2>(list_of_tuples[i]));
        if (candidate<mini_dist_wall){
            mini_dist_wall = candidate;
            mini_idx = i;
        }
    }

    Vector2D wall_vector = std::get<0>(list_of_tuples[mini_idx]);
    //double Angle = transform->velocity.angle(wall_vector);
    //const double pi = M_PI;
    //angle may not be needed

    if (wall_vector.x == 0){
        transform->velocity.x *= -1;
    }else if (wall_vector.y == 0){
        transform->velocity.y *= -1;
    }
    collisionCooldown = collisionCooldownMax;
}


void EnemyMovement::returnToInitialPosition()
{ // PAY ATTETION WITH VECTOR2D OPERATIONS.
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


void EnemyMovement:: explosion(){
    std::cout << "Explosion" << std::endl;
    dist_from_player = calculateDistanceToPlayer();
    std::cout << dist_from_player << std::endl;

    exploded = true;
    startExpTime  = SDL_GetTicks();
    knock_direction = Vector2D(playerTransform->position.x - transform->position.x, playerTransform->position.y - transform->position.y).Normalize();
    if (dist_from_player <explosion_radius){
        in_range = true;
        playerStats->SubtractHealth(10);

    }
    //stats->SubtractHealth(10); SubstractHealth from enemy (crashes for some reasons)

}

void EnemyMovement::update()
{

    if (exploded){
        Uint32 currentTime = SDL_GetTicks(); // Get the current time
        Uint32 delay = currentTime - startExpTime; // Calculate elapsed time since explosion started
        if (in_range){
            if (delay <= 200)
            {
                    if (delay >= 100)
                    {
                        if (delay <= 140)
                        {
                            playerTransform->position.x += knock_direction.x * 20;
                            playerTransform->position.y += knock_direction.y * 20;
                        }
                        else if (delay <= 180)
                        {
                            playerTransform->position.x += knock_direction.x * 10;
                            playerTransform->position.y += knock_direction.y * 10;
                        }
                        else
                        {
                            playerTransform->position.x += knock_direction.x * 5;
                            playerTransform->position.y += knock_direction.y * 5;
                        }
                    }
            }
        }

    }

    // Note that we only have to update velocities as they will influence the update of the position in the transform component
    if (collisionCooldown > 0) {
        collisionCooldown--;
        return; // Skip collision checks
    }
    if (enemyType == 1){
        swordsmanBehavior();
    }
    else if (enemyType == 2){
        creeperBehavior();
    }
}


float EnemyMovement::calculateDistanceToPlayer() const
{
    return transform->position.distance(playerTransform->position);
}

float EnemyMovement::calculateDistanceFromInitialPos() const
{
    return transform->position.distance(initial_position);
}

void EnemyMovement:: creeperBehavior()
{

    if (exploded == false){
    moveTimer++;
    if (moveTimer>=moveInterval){ // If long enough for velocity changes
        moveTimer = 0;
        float dist_from_player = calculateDistanceToPlayer(); // Distance enemy-player
        float dist_from_initial_position= calculateDistanceFromInitialPos(); //Distance of enemy's actual position- enemy's initial position
        float dist_to_explode = 80;
        if (arming) {
            // Creeper is in the countdown phase for explosion (add animation)
            std::cout << "Arming" << std::endl;
            armingTimer++;
            if (armingTimer >= armingDuration) {
                explosion();            }
        }

        else if (dist_from_player < dist_to_explode) {
             // If close enough, stop and start the countdown
             transform->velocity.x = 0;
             transform->velocity.y = 0;
             arming = true; // Start the explosion countdown
            }



        else if (dist_from_player < radius_of_attack) { // If player close enough from the enemy
            //std::cout << "In radius of attack" << std::endl;
            if (dist_from_initial_position < radius_of_pursuit){ // and If the enemy is close enough form its initial position to chase the enemy
                //std::cout << "In radius of pursuit" << std::endl;
                // Enemy chases the player
                //std::cout << "Chase mode active" << std::endl;

                // Inside the update method, within the chase logic
                int xDirection = 0;  // Start with no movement
                int yDirection = 0;

                float tolerance = 0; // Define a tolerance interval

                // Determine horizontal direction
                float xDistance = std::abs(playerTransform->position.x - transform->position.x);
                if (xDistance > mindist_enemy_player && xDistance > tolerance) {
                    if (playerTransform->position.x > transform->position.x) {
                            xDirection = 1;  // Player is to the right of the enemy, move right
                    } else {
                            xDirection = -1; // Player is to the left of the enemy, move left
                        }
                    }
                 else {
                     xDirection = 0; // Enemy is within tolerance range, do not move horizontally
                    }

                // Determine vertical direction
                float yDistance = std::abs(playerTransform->position.y - transform->position.y);
                if (yDistance > mindist_enemy_player && yDistance > tolerance) {
                    if (playerTransform->position.y > transform->position.y) {
                            yDirection = 1;  // Player is below the enemy, move down
                    } else {
                            yDirection = -1; // Player is above the enemy, move up
                    }
                 }
                 else {
                    yDirection = 0; // Enemy is within tolerance range, do not move vertically
                 }

                transform->velocity.x = xDirection * velocityScale;
                transform->velocity.y = yDirection * velocityScale;


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

}

void EnemyMovement:: swordsmanBehavior(){
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

                    // Inside the update method, within the chase logic
                    int xDirection = 0;  // Start with no movement
                    int yDirection = 0;

                    float tolerance = 0; // Define a tolerance interval

                    // Determine horizontal direction
                    float xDistance = std::abs(playerTransform->position.x - transform->position.x);
                    if (xDistance > mindist_enemy_player && xDistance > tolerance) {
                            if (playerTransform->position.x > transform->position.x) {
                                xDirection = 1;  // Player is to the right of the enemy, move right
                            } else {
                                xDirection = -1; // Player is to the left of the enemy, move left
                            }
                    }
                    else {
                            xDirection = 0; // Enemy is within tolerance range, do not move horizontally
                    }

                    // Determine vertical direction
                    float yDistance = std::abs(playerTransform->position.y - transform->position.y);
                    if (yDistance > mindist_enemy_player && yDistance > tolerance) {
                            if (playerTransform->position.y > transform->position.y) {
                                yDirection = 1;  // Player is below the enemy, move down
                            } else {
                                yDirection = -1; // Player is above the enemy, move up
                            }
                    }
                    else {
                            yDirection = 0; // Enemy is within tolerance range, do not move vertically
                    }

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
