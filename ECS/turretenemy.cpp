#include "turretenemy.h"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "math.h"

TurretEnemy::TurretEnemy(int r, int s, int d)
{
    radius = r;
    speed = s;
    damage = d;
}

void TurretEnemy::init()
{
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
}

double distance(Vector2D e1, Vector2D e2)
{
    return std::sqrt(std::pow(e2.x - e1.x, 2) + std::pow(e2.y - e1.y, 2));
}

void TurretEnemy::update()
{
    if(distance(transform->position,playertransform->position)<=radius)
    {
        CreateProjectile();
    }
}

void TurretEnemy::CreateProjectile()
{

}
