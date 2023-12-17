#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "ECS.h"
#include "Components.h"
#include <string.h>
#include <SDL.h>

class WeaponComponent : public Component
{
public:
    WeaponComponent(){};

    ~WeaponComponent() override = default;

    int attack(SDL_Event event)
    {
        if(event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_z:
                std::cout << "Hello" << std::endl;
            }
        }
        return 0;
    }

    int closeAttack()
    {

        return 0;
    }
private:
    std::string name = "Standard weapon ";
    int damage = 0;
};

#endif // WEAPONCOMPONENT_H
