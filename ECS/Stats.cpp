#include "Stats.h"

void Stats::AddHealth(int i)
{
    this->health += i;
}

void Stats::SubtractHealth(int i)
{
    this->health -= i;
}
void Stats::Damage(Stats& entity1, Stats& entity2)
{
    entity2.SubtractHealth(entity1.get_damage());
}
