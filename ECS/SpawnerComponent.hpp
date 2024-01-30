#ifndef SPAWNER_COMPONENT_H
#define SPAWNER_COMPONENT_H

#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "EnemyMovement.hpp"
#include <vector>

class Armor;

class SpawnerComponent : public Component {
public:
    SpawnerComponent(Manager& m, Uint32 interval, int max_enemies, TransformComponent* playerTrans, Stats* playerstats, ColliderComponent* playerCol, Armor* playerArmor);

    void init() override;
    void update() override;
    void spawnEnemy();
    std::vector<Entity*> spawnedEnemies;

private:
    Uint32 spawnInterval;
    int max_spawn;
    Uint32 lastSpawnTime;
    bool hasSpawned;
    TransformComponent* spawnerTransform;
    TransformComponent* playerTransform;
    ColliderComponent* playerCollider;
    Stats* playerStats;
    Armor* playerArmor;
    Manager& manager;
};

#endif // SPAWNER_COMPONENT_H
