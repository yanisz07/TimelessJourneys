#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.hpp"
#include "SDL_ttf.h"
#include "world.hpp"

class AssetManager
{
    friend Manager;
public:
    AssetManager(Manager* man);
    ~AssetManager();

    //gameobjects
    Manager* manager;

    World world;
    void loadWorld(std::string path);

    void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, bool player, int dam=5);


    //texture management
    void AddTexture(std::string id, const char* path);
    SDL_Texture* GetTexture(std::string id);

    void AddFont(std::string id, std::string path, int fontSize);
    TTF_Font* GetFont(std::string id);

private:


    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;

};

#endif // ASSETMANAGER_H
