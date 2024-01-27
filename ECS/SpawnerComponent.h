#ifndef SPAWNER_COMPONENT_H
#define SPAWNER_COMPONENT_H

#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "EnemyMovement.hpp"
#include <vector>

class SpawnerComponent : public Component {
public:
    SpawnerComponent(Manager& m, Uint32 interval, TransformComponent* playerTrans, Stats* playerstats);

    void init() override;
    void update() override;
    void spawnEnemy();
    std::vector<Entity*> spawnedEnemies;

private:
    Uint32 spawnInterval;
    Uint32 lastSpawnTime;
    bool hasSpawned;
    TransformComponent* spawnerTransform;
    TransformComponent* playerTransform;
    Stats* playerStats;
    Manager& manager;
};

#endif // SPAWNER_COMPONENT_H
