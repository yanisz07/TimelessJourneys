#include "AssetManager.hpp"
#include "ECS/Components.hpp"

//stores Assets and fonts in maps using string IDs

AssetManager::AssetManager(Manager *man) : manager(man)
{
    manager->assetManager = this;
}

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

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, bool player)
{
    auto& projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<SpriteComponent>(false, "player");
    projectile.getComponent<SpriteComponent>().setActions();
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

