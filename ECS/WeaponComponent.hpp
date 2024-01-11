#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "ECS.hpp"
#include "../timer.hpp"
#include "../game.hpp"
#include <string.h>
#include <SDL.h>


extern Manager manager; //not the same as in game.cpp

extern std::filesystem::path projectDir;


//forward decleration
class TransformComponent;
class SpriteComponent;
class ColliderComponent;
class Stats;
//end

class WeaponComponent : public Component
{
public:
    Timer timer;
    std::string attack;
    bool melee;
    SDL_Texture *texture;
    int damage = 0;
    Mix_Chunk* swordSwooshSound;

    WeaponComponent(Manager *man);

    ~WeaponComponent() override = default;

    int equip();

    void init() override;

    void update() override;

    int rangeAttack();

    int roundAttack();

    int frontAttack();

    int DoDamage(Stats& entity1);

    void draw() override;

private:
    std::string name = "Standard weapon ";

    TransformComponent* entityTransform;
    SpriteComponent* sprite;

    Uint32 reloadTime = 400;
    Manager* manager;


};

#endif // WEAPONCOMPONENT_H
