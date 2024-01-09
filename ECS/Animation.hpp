#ifndef ANIMATION_H
#define ANIMATION_H
#include <string>
#include "../timer.hpp"

struct Animation
{
    int wh;
    int index;
    int frames;
    int speed;
    int repeat = -1;
    std::string spriteName;

    Timer timer;

    Animation() {}
    Animation(int wh, int i, int f, int s, std::string spriteName);
};

#endif // ANIMATION_H
