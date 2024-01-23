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
//end

class Canon : public Component
{
public:
    Canon(int r, int s, int d,Uint32 rt, Manager* man, TransformComponent* player);

    void init() override;

    void update() override;

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
    Uint32 reloadTime;
};

#endif // CANON_H
