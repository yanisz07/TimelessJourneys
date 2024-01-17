#include "items.h"

Item::Item(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName)
    : is_equipped(equipped), location(loc), spritePath(path), name(itemName) {}

void Item::displayInfo() const {
    std::cout << "Item Info:" << std::endl;
    std::cout << "Equipped: " << is_equipped << std::endl;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Sprite Path: " << spritePath << std::endl;
    std::cout << "Name: " << name << std::endl;
}

Armor::Armor(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName,
             float dmgMult, int healthInc)
    : Item(equipped, loc, path, itemName), dmg_multiplier(dmgMult), health_increase(healthInc) {}

void Armor::displayInfo() const {
    Item::displayInfo();
    std::cout << "Armor Info:" << std::endl;
    std::cout << "Damage Multiplier: " << dmg_multiplier << std::endl;
    std::cout << "Health Increase: " << health_increase << std::endl;
}

Melee::Melee(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName, float dmgMult)
    : Item(equipped, loc, path, itemName), dmg_multiplier(dmgMult) {}

void Melee::displayInfo() const {
    Item::displayInfo();
    std::cout << "Melee Info:" << std::endl;
    std::cout << "Damage Multiplier: " << dmg_multiplier << std::endl;
}

RangedWeapon::RangedWeapon(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName,
                           float dmgMult, int rng, int arrowSpeed, int shootingSpeed)
    : Item(equipped, loc, path, itemName), dmg_multiplier(dmgMult), range(rng), speed_arrow(arrowSpeed), speed_shooting(shootingSpeed) {}

void RangedWeapon::displayInfo() const {
    Item::displayInfo();
    std::cout << "Ranged Weapon Info:" << std::endl;
    std::cout << "Damage Multiplier: " << dmg_multiplier << std::endl;
    std::cout << "Range: " << range << std::endl;
    std::cout << "Arrow Speed: " << speed_arrow << std::endl;
    std::cout << "Shooting Speed: " << speed_shooting << std::endl;
}
