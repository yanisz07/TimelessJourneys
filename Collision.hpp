#ifndef COLLISION_H
#define COLLISION_H

#include "SDL.h"

class ColliderComponent;

class Collision
{
public:
    /*Access Align Bounding Box*/
    static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
    static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
    static bool SAT(const SDL_Rect& recA, double angleA, const SDL_Rect& recB, double angleB);
    static bool SAT(const ColliderComponent& colA, const ColliderComponent& colB);
    static bool CheckCollision(const ColliderComponent& colA, const ColliderComponent& colB);
};

#endif // COLLISION_H
