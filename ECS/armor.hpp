#ifndef ARMOR_H
#define ARMOR_H

#include "ECS.hpp"
#include <string.h>
#include <SDL.h>

class SpriteComponent;

class Armor : public Component
{
public:

    Armor();

    ~Armor() override = default;

    void init() override;

    std::string get_type();

    void set_type(std::string t);

private:
    std::string type;
    int armor;
    SpriteComponent* sprite;
};

#endif // ARMOR_H
