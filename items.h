#pragma once

#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>
#include "ECS/Animation.hpp"
#include "ECS/Animation.hpp"
#include "SDL_image.h"
#include <map>

class Item {
public:
    bool is_equipped;
    std::string location;
    std::string spritePath;
    std::string name;
    SDL_Texture* icon = nullptr;
    Animation animation = {0,0,0,0,0,"Bow"};

    Item(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName);

    virtual void displayInfo() const;
    virtual void use() {
                std::cout << "Using item: " << name << std::endl;
    }
};

class Hand : public Item {
public:
    Hand(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName);

    void displayInfo() const override;
    void use() override {
                std::cout << "Attacking with hand: " << name << std::endl;
    }
};

class ArmorItem : public Item {
public:
    float dmg_multiplier;
    int health_increase;

    ArmorItem(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName,
          float dmgMult, int healthInc);

    void displayInfo() const override;
    void use() override {
        std::cout << "Equipping armor: " << name << std::endl;
    }
    };

class Melee : public Item {
public:
    float dmg_multiplier;

    Melee(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName, float dmgMult);

    void displayInfo() const override;
    void use() override {
    std::cout << "Attacking with melee weapon: " << name << std::endl;
    }
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
    void use() override {
        std::cout << "Shooting with ranged weapon: " << name << std::endl;
}
};





#endif // ITEM_HPP
