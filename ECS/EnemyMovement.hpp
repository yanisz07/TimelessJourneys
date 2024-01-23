#ifndef ENEMYMOVEMENT_H
#define ENEMYMOVEMENT_H

#include "ECS.hpp"
#include <SDL.h>
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
    Stats* playerStats;
    Stats* stats;
    int collisionCooldown = 0;
    const int collisionCooldownMax = 10;
    int moveTimer = 0;
    int moveInterval = 40;
    float velocityScale = 1;
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
    bool in_range = false;
    float explosion_radius = 200;
    Uint32 startExpTime;
    Vector2D knock_direction;


    Vector2D initial_position;

    EnemyMovement(int enemy_type, float radius_1, float radius_2, float radius_3, float distance_1, TransformComponent* playerTrans, Stats* playerstats, Stats* e_stats);

    void init() override;

    void onCollision(SDL_Rect collider_rect);

    void returnToInitialPosition();

    void update() override;

    float calculateDistanceToPlayer() const;

    float calculateDistanceFromInitialPos() const;

    void creeperBehavior();

    void swordsmanBehavior();

    void explosion();

};

#endif // ENEMYMOVEMENT_H
