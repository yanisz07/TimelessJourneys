#include "AssetManager.h"
#include "ECS/Components.h"

//stores Assets and fonts in maps using string IDs

AssetManager::AssetManager(Manager *man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::loadWorld(std::string path)
{
    world.loadWorld(path);
    for (auto iter1 = world.Characters.begin(); iter1 != world.Characters.end(); ++iter1)
    {
        for(auto iter2 = iter1->second.Sprites.begin(); iter2 != iter1->second.Sprites.end(); ++iter2)
        {
            AddTexture(iter2->first,iter2->second.path.c_str());
        }
    }
}

void AssetManager::AddTexture(std::string id, const char *path)
{
    textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
    return textures[id];
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
    auto& projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<SpriteComponent>(id,false);
    projectile.addComponent<ProjectileComponent>(range,speed, vel);
    projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
    return fonts[id];
}

