#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

//#include "Components.hpp"
#include "../Vector2D.hpp"

class TransformComponent : public Component
{

public:

    Vector2D position;
    Vector2D velocity;

    int height = 32;
    int width = 32;
    int scale = 1;

    int y_direction = 0;
    int x_direction = 1;

    int speed = 3;

    TransformComponent()
    {
        position.Zero();
    }

    TransformComponent(int sc)
    {
        position.x = 800;
        position.y = 640;
        scale = sc;
    }

    TransformComponent(double x, double y)
    {
        position.x = x;
        position.y = y;
    }

    TransformComponent(double x, double y, int h, int w, int sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }

    TransformComponent(double x, double y, int h, int w, int sc, int speed)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
        this->speed = speed;
    }

    void set_directions (int x, int y)
    {
        x_direction=x;
        y_direction=y;
    }

    void init() override
    {
        velocity.Zero();
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }

};


#endif // POSITIONCOMPONENT_H
