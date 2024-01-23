#ifndef TURRETENEMY_H
#define TURRETENEMY_H

#include "ECS.hpp"
#include "../game.hpp"
#include "../timer.hpp"
#include "../Vector2D.hpp"
#include <string.h>
#include <SDL.h>
#include "../game.hpp"

//forward declaration
class TransformComponent;
class ColliderComponent;
class ProjectileComponent;
//end

class TurretEnemy : public Component
{
public:
    TurretEnemy(int r, int s, int d,Uint32 rt, Manager* man, TransformComponent* player);

    void init() override;

    void update() override;

    void draw() override;

    void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam, double angle);

private:
    Manager* manager;
    TransformComponent* transform;
    TransformComponent* playertransform;
    int radius;
    int speed;
    int damage;
    Vector2D direction;
    Timer timer;
    Timer timer1;
    Uint32 reloadTime;

    SDL_Texture* tex1;
    Animation animation1 = Animation(70,76,0,4,150,"Turret");
    SDL_Rect srcR1, destR1;
    SDL_Texture* tex2;
};

#endif // TURRETENEMY_H
