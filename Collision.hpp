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
};

#endif // COLLISION_H
