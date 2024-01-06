#ifndef SWORD_H
#define SWORD_H
#include "Components.hpp"
class Sword : public WeaponComponent
{
public:

    Sword(Manager *man) : WeaponComponent(man)
    {
        melee = true;
        attack = "sword";
    }
};


#endif // SWORD_H
