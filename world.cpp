#include "world.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"

World::World(){
    std::cout << "World created" << std::endl;
}

Action::Action(nlohmann::json data, std::string sName)
{
    y_0 = data["values"][0];
    spriteName = sName;
    number_frames = data["values"][1];
    std::cout << "Action created: " << data["name"] << std::endl;
}


Sprite::Sprite(nlohmann::json data)
{
    path = data["path"].get<std::string>();
    std::cout << "Sprite created: " << data["name"] << std::endl;
}

Character::Character(nlohmann::json data){

    for(int i=0; i<size(data["sprites"]);i++)
    {
        nlohmann::json spriteData = data["sprites"][i];
        std::string sName = spriteData["name"].get<std::string>();
        Sprites.emplace(sName,Sprite(spriteData));

        for(int i=0; i<size(spriteData["actions"]);i++)
        {
            nlohmann::json actionData = spriteData["actions"][i];
            std::string aName = actionData["name"].get<std::string>();
            Actions.emplace(aName,Action(actionData,sName));
        }
    }
    std::cout << "Character created" << std::endl;
}
int World::loadWorld(std::string path)
{
    std::ifstream jsonFileStream(path);
    nlohmann::json jsonData = nlohmann::json::parse(jsonFileStream);
    for (int i = 0; i < size(jsonData["characters"]); ++i)
    {
        nlohmann::json characterData = jsonData["characters"][i];
        std::string name = characterData["name"].get<std::string>();
        Characters.emplace(name,Character(characterData));
    }

    return 0;
}
