#ifndef RANGE_WEAPON_H
#define RANGE_WEAPON_H

#include "ECS.hpp"
#include "../game.hpp"
#include "../timer.hpp"
#include "../Vector2D.hpp"
#include <string.h>
#include <SDL.h>

//forward decleration
class TransformComponent;
class SpriteComponent;
class ColliderComponent;
class Stats;
//end

class Range_Weapon : public Component
{
public:
    Timer timer;
    SDL_Texture* texture;
    std::string spritePath;

    //do an Animation to store this
    int frames = 4;
    int frame = 0;
    int index = 0;
    int speed = 100;
    int width = 32;
    int height = 20;
    //

    SDL_Rect srcR,destR;

    bool is_attacking = false;
    bool is_equiped = false;

    Vector2D direction;

    Range_Weapon(Manager* man);

    ~Range_Weapon() override = default;

    void init() override;

    void update() override;

    int rangeAttack();

    int DoDamage(Stats& entity1);

    void update_range_weapon();

    void CreateArrow(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam);

    void draw() override;

private:
    int damage = 5;
    TransformComponent* transform;
    SpriteComponent* sprite;
    Manager* manager;
    Uint32 reloadTime = 400;
};

#endif // RANGE_WEAPON_H
