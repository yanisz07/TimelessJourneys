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
        damage = 5;
        hit = false;
        hit_type = false;
        hit_time = Uint32(0);
        hit_direction = Vector2D();
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

    bool is_hit()
    {
        return hit;
    }

    bool type_hit()
    {
        return hit_type;
    }

    Uint32 time_hit()
    {
        return hit_time;
    }

    Vector2D direction_hit()
    {
        return hit_direction;
    }


    void set_hit(bool hit)
    {
        this->hit = hit;
    }

    void set_type_hit(bool hit_type)
    {
        this->hit_type = hit_type;
    }

    void set_hit_time(Uint32 hit_time)
    {
        this->hit_time = hit_time;
    }

    void set_hit_direction(Vector2D hit_direction)
    {
        this->hit_direction = hit_direction;
    }

private:
    int health;
    int damage;
    bool hit;
    bool hit_type; //false -> projectile, true -> melee
    Uint32 hit_time;
    Vector2D hit_direction;
};

#endif // STATS_H
