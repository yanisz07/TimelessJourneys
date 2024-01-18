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
    Mix_Chunk* bowSound;

    //do an Animation to store this
    int frames = 4;
    int frame = 0;
    int index = 0;
    int animspeed = 100;
    int width = 16;
    int height = 16;
    //

    SDL_Rect srcR,destR;

    bool is_attacking = false;
    bool is_equiped = false;

    Vector2D direction;

    Range_Weapon(Manager* man);

    ~Range_Weapon() override = default;

    int get_damage(){return damage;}
    int get_range(){return range;}
    int get_speed(){return speed;}
    Uint32 get_reload_time(){return reloadTime;}

    void set_damage(int dmg){damage=dmg;}
    void set_range(int r){range=r;}
    void set_speed(int s){speed=s;}
    void set_reload_time(Uint32 t){reloadTime=t;}

    void init() override;

    void update() override;

    int rangeAttack();

    void update_range_weapon();

    void CreateArrow(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int w, int h, int sc, int dam, double angle);

    void draw() override;

private:
    int damage = 3;
    int range = 400;
    int speed = 1;
    TransformComponent* transform;
    SpriteComponent* sprite;
    Manager* manager;
    Uint32 reloadTime = 400;
};

#endif // RANGE_WEAPON_H
