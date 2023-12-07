#ifndef COLLISION_H
#define COLLISION_H

#include "SDL.h"

class Collision
{
public:
    /*Access Align Bounding Box*/
    static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
};

#endif // COLLISION_H
