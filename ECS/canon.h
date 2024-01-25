#ifndef CANON_H
#define CANON_H

#include "ECS.hpp"
#include "../game.hpp"
#include "../timer.hpp"
#include "../Vector2D.hpp"
#include <string.h>
#include <SDL.h>

//forward declaration
class TransformComponent;
class ColliderComponentCircle;
class ProjectileComponent;
class SpriteComponent;
//end

class Canon : public Component
{
public:
    Canon(int r, int s, int d,Uint32 rt, Manager* man, TransformComponent* player);

    void init() override;

    void update() override;

    void draw() override;

    void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam);

private:
    Manager* manager;
    TransformComponent* transform;
    TransformComponent* playertransform;
    SpriteComponent* sprite;
    int radius;
    int speed;
    int damage;
    Vector2D direction;
    Timer timer;
    Uint32 reloadTime;
    double theta;
    bool is_attacking = false;
    bool check_anim=false;
    bool check_anim2=false;
    bool draw_anim2=false;

    SDL_Texture* tex1;
    Animation animation1 = Animation(14,13,0,5,100,"Canon_Bullet");
    SDL_Rect srcR1, destR1;
    Timer timer1;
};

#endif // CANON_H
