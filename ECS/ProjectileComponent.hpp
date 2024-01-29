#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "ECS.hpp"
#include "Components.hpp"
#include "../Vector2D.hpp"

class Stats;

class ProjectileComponent : public Component
{
public:

    ProjectileComponent(int rng, int sp, Vector2D vel, int dam = 5) : range(rng), speed(sp), velocity(vel), damage(dam)
    {timer.start();}
    ~ProjectileComponent()
    {}

    void init() override;

    int get_damage(){return damage;}
    int get_range(){return range;}
    int get_speed(){return speed;}

    void update() override;

    void DoDamage(Stats &entity1,Stats &entity2);
    void DoDamage(Stats &entity1);

private:

    TransformComponent* transform;
    int damage;
    int range;
    int speed;
    double distance;
    Vector2D velocity;
    Timer timer;

};

#endif // PROJECTILE_H
