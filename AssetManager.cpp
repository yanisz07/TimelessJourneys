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
        for(auto iter2_1 = iter1->second.Sprites.begin(); iter2_1 != iter1->second.Sprites.end(); ++iter2_1)
        {
            AddTexture(iter2_1->first,iter2_1->second.path.c_str());
        }
        for(auto iter2_2 = iter1->second.Attacks.begin();iter2_2 != iter1->second.Attacks.end(); ++iter2_2 )
        {
            for(auto iter3 = iter2_2->second.Sprites.begin(); iter3 != iter2_2->second.Sprites.end(); ++iter3)
            {
                AddTexture(iter3->first,iter3->second.path.c_str());
            }
        }
    }
}

void AssetManager::AddTexture(std::string id, const char *path)
{
    if(textures.count("id"))
    {
        textures[id] = TextureManager::LoadTexture(path);
    }
    else{
    textures.emplace(id, TextureManager::LoadTexture(path));
    }
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
    return textures[id];
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, bool player, int dam)
{
    auto& projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y);
    projectile.addComponent<SpriteComponent>(false, id);
    projectile.getComponent<SpriteComponent>().setActions();
    projectile.addComponent<ProjectileComponent>(range,speed,vel,dam);

    projectile.addComponent<ColliderComponent>(id);
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

