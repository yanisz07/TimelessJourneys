#ifndef TURRETENEMY_H
#define TURRETENEMY_H

#include "ECS.hpp"
#include "../game.hpp"
#include "../timer.hpp"
#include "../Vector2D.hpp"
#include <string.h>
#include <SDL.h>

//forward declaration
class TransformComponent;
class SpriteComponent;
class ColliderComponent;
//end

class TurretEnemy : public Component
{
public:
    TurretEnemy(int r, int s, int d);

    void init() override;

    void update() override;

    void CreateProjectile();

private:
    TransformComponent* transform;
    TransformComponent* playertransform;
    SpriteComponent* sprite;
    int radius;
    int speed;
    int damage;
    Vector2D direction;
    Timer timer;
};

#endif // TURRETENEMY_H
