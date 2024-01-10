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
