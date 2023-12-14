#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "ECS.h"
#include "Components.h"
#include <string.h>
#include <SDL.h>

class WeaponComponent : public Component
{
public:
    WeaponComponent()
    {
        damage = 0;
    }

    ~WeaponComponent() override = default;
    int attack(SDL_Event event)
    {
        return 0;
    }
private:
    std::string name;
    int damage;
};

#endif // WEAPONCOMPONENT_H
