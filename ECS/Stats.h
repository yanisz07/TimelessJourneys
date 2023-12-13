#ifndef STATS_H
#define STATS_H
#include "ECS.h"
#include "Components.h"

class Stats : public Component
{
public:
    Stats()
    {
        health = 10;
    }
    Stats(int h)
    {
        health = h;
    }
    ~Stats() override = default;

    int get_health() {return health;}

    void set_health(int i)
    {
        health = i;
    }

    void AddHealth(int i);
    void SubtractHealth(int i);

private:
    int health;
};

#endif // STATS_H
