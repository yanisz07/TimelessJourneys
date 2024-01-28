#include "canon.h"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponentCircle.hpp"
#include "math.h"
#include "ProjectileComponent.hpp"
#include "SpriteComponent.hpp"

Canon::Canon(int r, int s, int d, Uint32 rt, Manager* man, TransformComponent* player, int type)
{
    radius = r;
    speed = s;
    damage = d;
    manager = man;
    reloadTime = rt;
    playertransform = player;
    this->type = type;

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
        if(type == 0)
        {
            direction.x = 1;
            direction.y = 0;
        }
        if(type==1)
        {
            direction.x = -1;
            direction.y = 0;
        }
        is_attacking=true;
        check_anim=false;
        if(type==0)
        {
            sprite->Play("Attack_right",false,1);
        }
        if(type==1)
        {
            sprite->Play("Attack_left",false,1);
        }
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
                    if (type==0)
                    {
                        CreateProjectile(Vector2D(transform->position.x+transform->width*transform->scale+10,transform->position.y+12),direction,radius,speed,"canon_bullet",400,400,1,damage);
                    }
                    if(type==1)
                    {
                        CreateProjectile(Vector2D(transform->position.x-52,transform->position.y+12),direction,radius,speed,"canon_bullet",400,400,1,damage);
                    }
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
        if(type==0)
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
        if(type==1)
        {
            destR1.x = transform->position.x-52-Game::camera.x;
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
}
