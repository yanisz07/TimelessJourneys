#include "turretenemy.h"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "math.h"
#include "ProjectileComponent.hpp"

TurretEnemy::TurretEnemy(int r, int s, int d, Uint32 rt, Manager* man, TransformComponent* player)
{
    radius = r;
    speed = s;
    damage = d;
    manager = man;
    reloadTime = rt;
    playertransform = player;
}

void TurretEnemy::init()
{
    timer.setTimeOut(reloadTime);
    transform = &entity->getComponent<TransformComponent>();
}

double distance(Vector2D e1, Vector2D e2)
{
    return std::sqrt(std::pow(e2.x - e1.x, 2) + std::pow(e2.y - e1.y, 2));
}

void TurretEnemy::update()
{
    if(distance(transform->position,playertransform->position)<=radius && timer.timedOut())
    {
        direction.x = playertransform->position.x-transform->position.x;
        direction.y = playertransform->position.y-transform->position.y;
        direction.Normalize();
        //find angle between turret and player
        Vector2D e1 = Vector2D(1,0);
        double dot = direction.x*e1.x;
        double theta = std::acos(dot);
        theta = theta * (180.0 / M_PI);
        if (direction.y < 0)
        {
            theta=-theta;
        }
        CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale/2,transform->position.y+transform->height*transform->scale/2),direction,radius,speed,"enemy_arrow",15,7,2,damage,theta);
        timer.setTimeOut(reloadTime);
    }
}

void TurretEnemy::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam, double angle)
{
    auto& arrow(manager->addEntity());
    arrow.addComponent<TransformComponent>(pos.x, pos.y, h, w, sc);
    arrow.addComponent<SpriteComponent>(id);
    arrow.getComponent<SpriteComponent>().setAngle(angle);
    arrow.addComponent<ProjectileComponent>(range,speed,vel,dam);
    arrow.addComponent<ColliderComponent>(id,pos.x,pos.y,w,h);
    arrow.getComponent<ColliderComponent>().SetAngle(angle);
    arrow.addGroup(Game::groupEnemyProjectiles);
}