#include "armor.h"
#include "SpriteComponent.hpp"

Armor::Armor()
{
    type="";
    armor=0;
}

void Armor::init()
{
    sprite = &entity->getComponent<SpriteComponent>();
}

std::string Armor::get_type()
{
    return type;
}

void Armor::set_type(std::string t)
{
    type=t;
}
