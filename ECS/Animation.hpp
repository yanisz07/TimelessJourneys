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
    Animation(int wh, int i, int f, int s, std::string spriteName)
    {
        this->wh = wh; //square objects for now;
        index = i; //x value
        frames = f;
        speed = s;
        this->spriteName = spriteName;
        timer.start();
        timer.setTimeOut(frames*speed);
    }
};

#endif // ANIMATION_H
