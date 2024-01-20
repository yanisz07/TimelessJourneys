#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>
#include "ECS/Animation.hpp"
#include "SDL_image.h"
#include <map>

class Item {
public:
    bool is_equipped;
    std::string location;
    std::string spritePath;
    std::string name;
    SDL_Texture* icon;

    Item(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName);

    virtual void displayInfo() const;
};

class Armor : public Item {
public:
    float dmg_multiplier;
    int health_increase;

    Armor(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName,
          float dmgMult, int healthInc);

    void displayInfo() const override;
};

class Melee : public Item {
public:
    float dmg_multiplier;

    Melee(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName, float dmgMult);

    void displayInfo() const override;
};

class RangedWeapon : public Item {
public:
    float dmg_multiplier;
    int range;
    int speed_arrow;
    int speed_shooting;

    RangedWeapon(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName,
                 float dmgMult, int rng, int arrowSpeed, int shootingSpeed);

    void displayInfo() const override;
};

#endif // ITEM_HPP
