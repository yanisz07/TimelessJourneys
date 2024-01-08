#ifndef PROJECTILE_H
#define PROJECTILE_H


//#include "ECS.hpp"
//#include "Stats.hpp"
#include "Components.hpp"
//#include "../Vector2D.hpp"

//class Stats;

class ProjectileComponent : public Component
{
public:

    ProjectileComponent(int rng, int sp, Vector2D vel, int dam = 5) : range(rng), speed(sp), velocity(vel), damage(dam)
    {}
    ~ProjectileComponent()
    {}

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
    }

    void update() override
    {
        distance += speed;

        if(distance > range)
        {
            //std::cout << "out of range" << std::endl;
            entity->destroy();
        }
        else if(transform->position.x > Game::camera.x + Game::camera.w ||
                transform->position.x < Game::camera.x ||
                transform->position.y > Game::camera.y + Game::camera.h ||
                transform->position.y < Game::camera.y)
        {
            //std::cout << "out of bounds" << std::endl;
            entity->destroy();
        }
    }

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
