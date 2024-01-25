#include "canon.h"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponentCircle.hpp"
#include "math.h"
#include "ProjectileComponent.hpp"

Canon::Canon(int r, int s, int d, Uint32 rt, Manager* man, TransformComponent* player)
{
    radius = r;
    speed = s;
    damage = d;
    manager = man;
    reloadTime = rt;
    playertransform = player;
}

void Canon::init()
{
    timer.setTimeOut(reloadTime);
    transform = &entity->getComponent<TransformComponent>();
}

double distance_to_player(Vector2D e1, Vector2D e2)
{
    return std::sqrt(std::pow(e2.x - e1.x, 2) + std::pow(e2.y - e1.y, 2));
}

void Canon::update()
{
    if(distance_to_player(transform->position,playertransform->position)<=radius && timer.timedOut())
    {
        direction.x = playertransform->position.x+13*playertransform->scale+(13/2)*playertransform->scale-(transform->position.x+transform->width*transform->scale/2);
        direction.y = playertransform->position.y+18*playertransform->scale+(19/2)*playertransform->scale-(transform->position.y+transform->height*transform->scale/2);
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
        CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale/2,transform->position.y+transform->height*transform->scale/2),direction,radius,speed,"canon_bullet",400,400,1,damage);
        timer.setTimeOut(reloadTime);
    }
}

void Canon::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam)
{
    auto& arrow(manager->addEntity());
    arrow.addComponent<TransformComponent>(pos.x, pos.y, h, w, sc);
    arrow.addComponent<SpriteComponent>(id);
    arrow.getComponent<SpriteComponent>().Set_Dest_Rect(40,40);
    arrow.addComponent<ProjectileComponent>(range,speed,vel,dam);
    arrow.addComponent<ColliderComponentCircle>(id,20);
    arrow.addGroup(Game::groupEnemyProjectiles);
}
