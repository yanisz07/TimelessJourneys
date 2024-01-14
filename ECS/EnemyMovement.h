#ifndef ENEMYMOVEMENT_H
#define ENEMYMOVEMENT_H

#include "ECS.hpp"
#include <stdlib.h>
#include <math.h>
#include "../Vector2D.hpp"
#include "Stats.hpp"


//forward decleration
class TransformComponent;
//end;

class EnemyMovement : public Component
{
public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    Stats* stats;
    int collisionCooldown = 0;
    const int collisionCooldownMax = 10;
    int moveTimer = 0;
    int moveInterval = 40;
    float velocityScale = 0.65;
    int enemyType;
    float radius_of_attack;
    float radius_of_displacement;
    float radius_of_pursuit;
    float mindist_enemy_player;
    float dist_from_player;  // Distance enemy-player
    float dist_from_initial_position; //Distance of enemy's actual position- enemy's initial position
    bool pursuit;
    int armingTimer = 0;
    int armingDuration = 4; // Duration for the creeper to arm itself
    bool arming = false;
    bool exploded = false;
    float explosion_radius = 200;

    Vector2D initial_position;

    EnemyMovement(int enemy_type, float radius_1, float radius_2, float radius_3, float distance_1, TransformComponent* playerTrans);

    void init() override;

    void onCollision();

    void returnToInitialPosition();

    void update() override;

    float calculateDistanceToPlayer() const;

    float calculateDistanceFromInitialPos() const;

    void creeperBehavior();

    void swordsmanBehavior();

    void explosion();

};

#endif // ENEMYMOVEMENT_H
