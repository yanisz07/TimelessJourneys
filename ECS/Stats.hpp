#ifndef STATS_H
#define STATS_H
#include "ECS.hpp"
#include "Components.hpp"

class Stats : public Component
{
public:
    Stats()
    {
        health = 10;
        damage_mult = 5.0; // TODO Should be changed to 1.0 when Damage is fixed for balance purposes.
        exp_worth = 50;
    }
    Stats(int h, float d = 1.0, int exp = 50)
    {
        health = h;
        damage_mult = d;
        exp_worth = exp;
    }
    Stats(bool pc)
    {
        if (pc) {
            player = pc;
            health = 50;
            damage_mult = 1.0;
            max_health = 50;
            experience = 0;
            level = 1;
        } else {
            health = 10;
            damage_mult = 5;
        }
    }
    Stats(int h, float d, int m, int exp, int lvl)
    { // The statistics here must be consistent! No check is being performed.
        health = h;
        damage_mult = d;
        player = true;
        max_health = m;
        experience = exp;
        level = lvl;
    }
    ~Stats() override = default;

    int get_health() {return health;}
    int get_damage_mult() {return damage_mult;}
    bool is_player() {return player;}
    int get_max_health() {return max_health;}
    int get_experience() {return experience;}
    int get_level() {return level;}

    void set_health(int i) {health = i;} // Should not be used on players! Or anyone really, only debug.

    void SubtractHealth(int); // Supports negative values for healing.
    void GainExp(int); // Handles level ups. Supports negative values. Will level down the player down to level 1 if necessary.
    void KillEntity(); // Only for enemies.
    void GameOver(); // Only for the player.

    static void Damage(Stats& entity1, Stats& entity2); // TODO weapons deal damage and multiply with damage_mult, not characters directly.

private:
    int health;
    float damage_mult; // The multiplier of the base damage.
    int exp_worth; // Only for enemies.
    bool player; // Stats pertaining to the player if true underneath.
    int max_health;
    int experience;
    int level;
};

#endif // STATS_H
