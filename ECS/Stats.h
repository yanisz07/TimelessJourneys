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
        damage = 5;
    }
    Stats(int h, int d)
    {
        health = h;
        damage = d;
    }
    ~Stats() override = default;

    int get_health() {return health;}
    int get_damage() {return damage;}

    void set_health(int i)
    {
        health = i;
    }

    void AddHealth(int i);
    void SubtractHealth(int i);

    static void Damage(Stats& entity1, Stats& entity2);

private:
    int health;
    int damage; //this is the melee damage, so at collision with the ennemy the ennemy takes dmg (later we need to implement a button to trigger that)
};

#endif // STATS_H
