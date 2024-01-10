#ifndef PROJECTILE_H
#define PROJECTILE_H


#include "ECS.hpp"
//#include "Stats.hpp"
#include "Components.hpp"
#include "../Vector2D.hpp"

//class Stats;

class ProjectileComponent : public Component
{
public:

    ProjectileComponent(int rng, int sp, Vector2D vel, int dam = 5) : range(rng), speed(sp), velocity(vel), damage(dam)
    {}
    ~ProjectileComponent()
    {}

    void init() override;

    void update() override;
   //void doDamage(Stats& enemyStatsComponent)
   //{
   // enemyStatsComponent.SubtractHealth(damage);
   //}

private:

    TransformComponent* transform;
    int damage;
    int range;
    int speed;
    int distance;
    Vector2D velocity;

};

#endif // PROJECTILE_H
