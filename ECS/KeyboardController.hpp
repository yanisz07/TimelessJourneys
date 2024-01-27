#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "armor.hpp"

//forward decleration
class Sword;
class Range_Weapong;
//end

class KeyboardController : public Component
{
public:
    TransformComponent* transform;

    SpriteComponent* sprite;

    Armor* armor;

    bool pressed_right = false;
    bool pressed_left = false;
    bool pressed_up = false;
    bool pressed_down = false;

    void init() override;

    void update() override;

};

#endif // KEYBOARDCONTROLLER_H
