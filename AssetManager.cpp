#include "AssetManager.h"
#include "ECS/Components.h"

//stores Assets and fonts in maps using string IDs

AssetManager::AssetManager(Manager *man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::AddTexture(std::string id, const char *path)
{
    textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
    return textures[id];
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, bool player)
{
    auto& projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<SpriteComponent>(id,false);
    projectile.addComponent<ProjectileComponent>(range,speed, vel);
    projectile.addComponent<ColliderComponent>(id);
    projectile.addComponent<Stats>(0,2);
    if (player)
    {
        projectile.addGroup(Game::groupPlayerProjectiles);
    }
    else
    {
        projectile.addGroup(Game::groupEnemyProjectiles);
    }
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
    return fonts[id];
}

