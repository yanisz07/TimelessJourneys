#include "Animation.hpp"

Animation::Animation(int wh, int i, int f, int s, std::string spriteName)
{
    this->wh = wh; //square objects for now;
    index = i; //x value
    frames = f;
    speed = s;
    this->spriteName = spriteName;
    timer.start();
    timer.setTimeOut(frames*speed);
}

Animation::Animation(int w, int h, int i, int f, int s, std::string spriteName)
{
    width = w;
    height = h;
    index = i; //x value
    frames = f;
    speed = s;
    this->spriteName = spriteName;
    timer.start();
    timer.setTimeOut(frames*speed);
}
