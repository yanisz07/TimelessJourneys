#ifndef WORLD_H
#define WORLD_H
#include "json.hpp"
#include <string>
#include <map>

class Action;
class Sprite;
class Attack;
class Character;


class World
{
public:
    std::map<std::string, Character> Characters;
    int loadWorld(std::string path);
    World();
};

class Character
{
public:
    std::map<std::string, Sprite> Sprites;
    std::map<std::string, Action> Actions;
    std::map<std::string, Attack> Attacks;
    Character(nlohmann::json data);
    Character();
};
class Sprite
{
public:
    Sprite(nlohmann::json data);
    std::string path;
};

class Action
{
public:
    int y_0 = 0;
    int number_frames = 0;
    std::string spriteName;
    Action();
    Action(nlohmann::json data, std::string sName);
};

class Attack
{
public:
    std::map<std::string, Sprite> Sprites;
    std::map<std::string, Action> Actions;
    Attack(nlohmann::json data);
    Attack();
};



#endif // WORLD_H
