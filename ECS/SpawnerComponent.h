#ifndef SPAWNER_COMPONENT_H
#define SPAWNER_COMPONENT_H

#include "ECS.hpp"
#include "TransformComponent.hpp"
#include <vector>
#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include "../Vector2D.hpp"
#include "Stats.hpp"
#include "../game.hpp"


class SpawnerComponent : public Component {
public:
    SpawnerComponent(Manager& m, Uint32 interval, int max, std::vector<Vector2D> points, TransformComponent* playerTrans, Stats* playerstats);

    TransformComponent* playerTransform;
    Stats* playerStats;
    void update() override;
    void spawnEnemy();
    bool hasSpawned = false ;


private:
    Uint32 spawnInterval;
    Uint32 lastSpawnTime;
    int maxEnemies;
    std::vector<Vector2D> spawnPoints;
    Manager& manager;
};

#endif // SPAWNER_COMPONENT_H
