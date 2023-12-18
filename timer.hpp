#ifndef TIMER_HPP
#define TIMER_HPP
#include <SDL.h>
#include <iostream>

class Timer
{
private:
    Uint32 startTime_;
    Uint32 partialTime_;
public:
    Timer()
    {
        std::cout << "Timer created successfully" << std::endl;
    }
    inline int start(){startTime_ = SDL_GetTicks(); partialTime_ = SDL_GetTicks(); return 0;}
    inline int partial(){partialTime_ = SDL_GetTicks(); return 0;}
    inline Uint32 getTimeStart(){return SDL_GetTicks() - startTime_;}
    inline Uint32 getTimeDelta(){return SDL_GetTicks() - partialTime_;}

};
#endif // TIMER_HPP
