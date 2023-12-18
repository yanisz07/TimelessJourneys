#ifndef ANIMATION_H
#define ANIMATION_H
#include <string>

struct Animation
{
    int index;
    int frames;
    int speed;
    std::string spriteName;

    Animation() {}
    Animation(int i, int f, int s, std::string spriteName)
    {
        index = i;
        frames = f;
        speed = s;
        this->spriteName = spriteName;
    }
};

#endif // ANIMATION_H
