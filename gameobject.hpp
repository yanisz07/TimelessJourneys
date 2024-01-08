#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "game.hpp"

enum WeaponType {
    NO_WEAPON,
    SWORD,
    CANON,
    // Add more weapon types here as needed
};

enum ArmourType {
    NO_ARMOUR,
    HELMET,
    CHESTPLATE,
    LEGGINGS,
    BOOTS,
    // Add more armour types here as needed
};

class GameObject
{
public:
    GameObject(const char* texturesheet, int x, int y);
    ~GameObject();

    void Update();
    void Render();

    void EquipWeapon(WeaponType weapon);
    void Attack();

private:
    int xpos;
    int ypos;

    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;

    WeaponType equippedWeapon;
    int weaponDamage;
    int weaponDurability;
};

#endif // GAMEOBJECT_H

