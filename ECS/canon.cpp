#include "canon.h"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponentCircle.hpp"
#include "math.h"
#include "ProjectileComponent.hpp"
#include "SpriteComponent.hpp"

Canon::Canon(int r, int s, int d, Uint32 rt, Manager* man, TransformComponent* player)
{
    radius = r;
    speed = s;
    damage = d;
    manager = man;
    reloadTime = rt;
    playertransform = player;

    std::string spritePath = (projectDir / ".." / "TimelessJourneys" / "assets" / "Canon" / "Explosion_anim.png").string();
    tex1 = IMG_LoadTexture(Game::renderer,spritePath.c_str());
    timer1.start();

    srcR1.w = animation1.width;
    srcR1.h = animation1.height;
    destR1.w = 42;
    destR1.h = 39;
}

void Canon::init()
{
    timer.setTimeOut(reloadTime);
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
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
        theta = std::acos(dot);
        theta = theta * (180.0 / M_PI);
        if (direction.y < 0)
        {
            theta=-theta;
        }
        is_attacking=true;
        check_anim=false;
        sprite->Play("Attack_right",false,1);
        timer.setTimeOut(reloadTime);
    }
    else if (is_attacking)
    {
        if(timer.timedOut())
        {
            is_attacking=false;
        }
        else
        {
            if (timer.getTimeOutStart() >= 500)
            {
                if (!check_anim)
                {
                    check_anim = true;
                    draw_anim2 = true;
                    check_anim2 = false;
                    timer1.start();
                }
            }
            if (timer.getTimeOutStart() >= 700)
            {
                if (!check_anim2)
                {
                    check_anim2 = true;
                    CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale+10,transform->position.y+12),direction,radius,speed,"canon_bullet",400,400,1,damage);
                }
            }
        }
    }
}

void Canon::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam)
{
    auto& arrow(manager->addEntity());
    arrow.addComponent<TransformComponent>(pos.x, pos.y, h, w, sc);
    arrow.addComponent<SpriteComponent>(id);
    arrow.getComponent<SpriteComponent>().Set_Dest_Rect(42,42);
    arrow.addComponent<ProjectileComponent>(range,speed,vel,dam);
    arrow.addComponent<ColliderComponentCircle>(id,20);
    arrow.addGroup(Game::groupEnemyProjectiles);
}

void Canon::draw()
{
    if (timer1.getTimeStart() > animation1.frames*animation1.speed - 25)
    {
        draw_anim2=false;
    }
    else if (draw_anim2)
    {
        destR1.x = transform->position.x+transform->width*transform->scale+10-Game::camera.x;
        destR1.y = transform->position.y+12-Game::camera.y;
        srcR1.y=0;
        int frame;
        frame = static_cast<int>((timer1.getTimeStart() / animation1.speed) % animation1.frames);
        animation1.index = frame;
        srcR1.x = animation1.width*animation1.index;
        if (SDL_RenderCopyEx(Game::renderer,tex1,&srcR1,&destR1,NULL,NULL,SDL_FLIP_NONE) == 0) {
            // The rendering was successful
            // Add any additional code you want to execute on success
        } else {
            // The rendering failed
            // Add error-handling code here
            const char* sdlError = SDL_GetError();
            // Print or handle the SDL error message as needed
            printf("SDL_RenderCopyEx failed: %s\n", sdlError);
        }
    }
}
