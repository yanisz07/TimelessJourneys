#include "turretenemy.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "math.h"
#include "ProjectileComponent.hpp"
#include "SpriteComponent.hpp"

TurretEnemy::TurretEnemy(int r, int s, int d, Uint32 rt, Manager* man, TransformComponent* player)
{
    radius = r;
    speed = s;
    damage = d;
    manager = man;
    reloadTime = rt;
    playertransform = player;

    std::string spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "propAssets" / "turret.png").string();
    tex1 = IMG_LoadTexture(Game::renderer,spritePath.c_str());

    srcR1.w = animation1.width;
    srcR1.h = animation1.height;
    destR1.w = 210;
    destR1.h = 228;
}

void TurretEnemy::init()
{
    timer.setTimeOut(reloadTime);
    timer1.start();
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
    setPriority(sprite->priority - 1);
}

double distance(Vector2D e1, Vector2D e2)
{
    return std::sqrt(std::pow(e2.x - e1.x, 2) + std::pow(e2.y - e1.y, 2));
}

void TurretEnemy::update()
{
    if (distance(transform->position,playertransform->position)<=radius && timer.timedOut())
    {
        direction.x = playertransform->position.x+13*playertransform->scale+(13/2)*playertransform->scale-(transform->position.x+transform->width*transform->scale/2);
        direction.y = playertransform->position.y+18*playertransform->scale+(19/2)*playertransform->scale-(transform->position.y+transform->height*transform->scale/2);
        direction.Normalize();
        //find angle between turret and player
        Vector2D e1 = Vector2D(1,0);
        double dot = direction.x*e1.x;
        theta = std::acos(dot);
        theta = theta * (180.0 / M_PI);
        if (direction.y < 0)
        {
            theta=-theta;
        }
        is_attacking=true;
        check_anim=false;
        if (theta<0)
        {
            theta+=360;
        }
        if (((0 <= theta) && (theta <= 45)) || ((315 <= theta) && (theta <= 360)))
        {
            sprite->Play("Attack_Side",true,1);
        }
        else if ((45 <= theta) && (theta <= 135))
        {
            sprite->Play("Attack_Down",false,1);
        }
        else if ((135 <= theta) && (theta <= 225))
        {
            sprite->Play("Attack_Side",false,1);
        }
        else
        {
            sprite->Play("Attack_Up",false,1);
        }
        timer.setTimeOut(reloadTime);
    }
    else if (is_attacking)
    {
        if(timer.timedOut())
        {
            is_attacking=false;
            sprite->Play("Idle_Down");
        }
        else
        {
            if (timer.getTimeOutStart() >= 600)
            {
                if (!check_anim)
                {
                    if (((0 <= theta) && (theta <= 45)) || ((315 <= theta) && (theta <= 360)))
                    {
                        CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale/2+10,transform->position.y+transform->height*transform->scale/2-10),direction,radius,speed,"enemy_arrow",15,7,2,damage,theta);
                    }
                    else if ((45 <= theta) && (theta <= 135))
                    {
                        CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale/2-10,transform->position.y+transform->height*transform->scale/2+10),direction,radius,speed,"enemy_arrow",15,7,2,damage,theta);
                    }
                    else if ((135 <= theta) && (theta <= 225))
                    {
                        CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale/2-20,transform->position.y+transform->height*transform->scale/2-10),direction,radius,speed,"enemy_arrow",15,7,2,damage,theta);
                    }
                    else
                    {
                        CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale/2-10,transform->position.y+transform->height*transform->scale/2-10),direction,radius,speed,"enemy_arrow",15,7,2,damage,theta);
                    }
                    check_anim = true;
                }
            }
        }
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

void TurretEnemy::draw()
{
    destR1.x = transform->position.x-57-Game::camera.x;
    destR1.y = transform->position.y-10-Game::camera.y;
    srcR1.y=0;
    int frame;
    frame = static_cast<int>((timer1.getTimeStart() / animation1.speed) % animation1.frames);
    if (timer1.getTimeStart() > animation1.frames*animation1.speed)
    {
        timer1.start();
    }
    animation1.index = frame;
    srcR1.x = animation1.width*animation1.index;
    SDL_RenderCopyEx(Game::renderer,tex1,&srcR1,&destR1,NULL,NULL,SDL_FLIP_NONE);
}
