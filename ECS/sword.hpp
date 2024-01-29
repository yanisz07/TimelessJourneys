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
#include "Animation.hpp"

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
    SDL_Texture* texture;
    Mix_Chunk* swordSwooshSound;

    std::string spritePath;
    Animation animation = Animation(276,539,0,5,100,"BlueSword");
    SDL_Rect srcR, destR;

    bool is_attacking = false;
    bool is_equiped = true;

    Vector2D direction;

    Sword(Manager *man);

    int equip();
    \
    float dmg_multiplier;

    void init() override;

    void update() override;

    int frontAttack();

    int DoDamage(Stats &entity1, Stats &entity2);

    void update_sword();

    void draw() override;

    void sword_equip(Inventory& inventory);

    void setSprite(std::string spritePath);

    void setDamage(int damage);

    void setName(std::string name);

    void setIcon(SDL_Texture* texture);


private:
    TransformComponent* entityTransform;
    std::string name = "Standard weapon ";
    Uint32 reloadTime = 400;
    Manager* manager;
    SpriteComponent* sprite;
    int damage = 5;
};


#endif // SWORD_H
