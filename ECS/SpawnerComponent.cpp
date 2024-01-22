#include "SpawnerComponent.h"
#include "SDL_timer.h"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "EnemyMovement.h"


SpawnerComponent::SpawnerComponent(Manager& m, Uint32 interval, int max, std::vector<Vector2D> points, TransformComponent* playerTrans, Stats* playerstats)
    : manager(m), spawnInterval(interval), maxEnemies(max), spawnPoints(points) {
    lastSpawnTime = SDL_GetTicks(), playerTransform=playerTrans, playerStats = playerstats; // Initialize with current time
}

void SpawnerComponent::update() {
    Uint32 currentTime = SDL_GetTicks();

    // If an enemy has been spawned, check if it's time to spawn again after 10 seconds
    if (hasSpawned && currentTime - lastSpawnTime > 10000) {
        hasSpawned = false; // Reset the flag to allow another spawn
    }

    // If we haven't spawned an enemy yet or it's time to spawn again
    if (!hasSpawned) {
        spawnEnemy();
        lastSpawnTime = currentTime; // Update the last spawn time
        hasSpawned = true; // Set the flag so no more enemies will be spawned until the interval passes
    }
}


void SpawnerComponent::spawnEnemy() {
    // Choose a random spawn point
    int index = rand() % spawnPoints.size();
    Vector2D spawnLocation = spawnPoints[index];

    // Spawn an enemy at the chosen location
    Entity& enemy = manager.addEntity();
    enemy.addComponent<TransformComponent>(1200,1000,128,128,1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(1,500,200,1200,60,playerTransform, playerStats, &enemyStats); //To be changed later on
    enemy.addGroup(Game::groupEnemies);
}


