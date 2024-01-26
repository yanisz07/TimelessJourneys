#include "items.hpp"
#include "TextureManager.hpp"
#include "game.hpp"
#include "ECS/sword.hpp"
#include "ECS/range_weapon.hpp"
#include "ECS/Stats.hpp"

Item::Item(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName)
    : is_equipped(equipped), location(loc), spritePath(path), name(itemName) {
    icon = TextureManager::LoadTexture(path.c_str());
}

void Item::displayInfo() const {
    std::cout << "Item Info:" << std::endl;
    std::cout << "Equipped: " << is_equipped << std::endl;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Sprite Path: " << spritePath << std::endl;
    std::cout << "Name: " << name << std::endl;
}

ArmorItem::ArmorItem(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName,
             float dmgMult, int healthInc)
    : Item(equipped, loc, path, itemName), dmg_multiplier(dmgMult), health_increase(healthInc) {}

void ArmorItem::displayInfo() const {
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

int Melee::use() {
    Inventory* inventory = Game::inventory;
    Sword& sword = inventory->game->assets->manager->getGroup(Game::groupPlayers)[0]->getComponent<Sword>();
    sword.setDamage(dmg_multiplier);
    sword.setSprite(spritePath);
    sword.setName(name);
    sword.setIcon(icon);
    return 0;

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

HealingPotion::HealingPotion(bool equipped, const std::string& loc, const std::string& path, const std::string& itemName, int effect)
    : Item(equipped, loc, path, itemName) {this->effect = effect;}

int HealingPotion::use(){
    Stats& stats = Game::inventory->game->assets->manager->getGroup(Game::groupPlayers)[0]->getComponent<Stats>();
    stats.addHealth(effect);
    return 1;

}

void HealingPotion::displayInfo() const {
    Item::displayInfo();
    std::cout << "Potion Info:" << std::endl;
    std::cout << "Healing Effect: " << effect << std::endl;
}

int RangedWeapon::use() {
    Inventory* inventory = Game::inventory;
    Range_Weapon& range_weapon = inventory->game->assets->manager->getGroup(Game::groupPlayers)[0]->getComponent<Range_Weapon>();
    range_weapon.set_damage(dmg_multiplier);
    range_weapon.set_sprite(spritePath);
    range_weapon.set_icon(icon);
    range_weapon.set_range(range);
    range_weapon.set_speed(speed_shooting);
    return 0;

}
