#ifndef SWORD_H
#define SWORD_H
#include "ECS.hpp"
#include <filesystem>
//#include "Components.hpp"
#include "../game.hpp"
#include "TransformComponent.hpp"
#include "../TextureManager.hpp"
#include "../AssetManager.hpp"
#include "../timer.hpp"
#include <string.h>
#include <SDL.h>

//forward decleration
class TransformComponent;
class SpriteComponent;
class ColliderComponent;
class Stats;
//end

class Sword : public Component
{
public:

    Timer timer;
    std::string attack;
    bool melee;
    SDL_Texture* texture;
    int damage = 0;
    Mix_Chunk* swordSwooshSound;

    std::string spritePath;
    int frames = 4;
    int frame = 0;
    int index = 0;
    int speed = 100;
    int width = 276;
    int height = 539;
    SDL_Rect srcR, destR;

    bool is_attacking = false;
    bool is_equiped = true;

    Vector2D direction;

    Sword(Manager *man);

    ~Sword() override = default;

    int equip();

    void init() override;

    void update() override;

    int frontAttack();

    int DoDamage(Stats& entity1);

    void update_sword();

    void draw() override;

private:
    TransformComponent* entityTransform;
    std::string name = "Standard weapon ";
    Uint32 reloadTime = 400;
    Manager* manager;
    SpriteComponent* sprite;
};


#endif // SWORD_H
