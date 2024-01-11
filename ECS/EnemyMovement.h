#ifndef ENEMYMOVEMENT_H
#define ENEMYMOVEMENT_H

#include "ECS.hpp"
#include <stdlib.h>
#include <math.h>
#include "../Vector2D.hpp"

//forward decleration
class TransformComponent;
//end;

class EnemyMovement : public Component
{
public:
    TransformComponent* transform;
    TransformComponent* playerTransform;
    int collisionCooldown = 0;
    const int collisionCooldownMax = 10;
    int moveTimer = 0;
    int moveInterval = 40;
    float velocityScale = 0.65;
    float radius_of_attack;
    float radius_of_displacement;
    float radius_of_pursuit;
    float mindist_enemy_player;
    bool pursuit;
    Vector2D initial_position;

    EnemyMovement(float radius_1, float radius_2, float radius_3, float distance_1, TransformComponent* playerTrans);

    void init() override;

    void onCollision();

    void returnToInitialPosition();

    void update() override;

    float calculateDistanceToPlayer() const;

    float calculateDistanceFromInitialPos() const;

};

#endif // ENEMYMOVEMENT_H
