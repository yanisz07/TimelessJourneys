#include "armor.hpp"
#include "SpriteComponent.hpp"

Armor::Armor()
{
    type="";
    armor=1;
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
    if (t == "Silver")
    {
        armor = 1.2;
    }
    else if (t == "Gold")
    {
        armor = 1.5;
    }
    else if (t == "Diamond")
    {
        armor = 2;
    }
    else
    {
        armor = 1;
    }
    type=t;
    std::cout << armor << std::endl;
}
