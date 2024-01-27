#include "SpawnerComponent.h"
#include "SDL_timer.h"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "EnemyMovement.hpp"


SpawnerComponent::SpawnerComponent(Manager& m, Uint32 interval, TransformComponent* playerTrans, Stats* playerstats)
    : manager(m), spawnInterval(interval), playerTransform(playerTrans), playerStats(playerstats), hasSpawned(false) {
    lastSpawnTime = SDL_GetTicks();
}

void SpawnerComponent::init() {
    spawnerTransform = &entity->getComponent<TransformComponent>();
}

void SpawnerComponent::update() {
    // Remove any references to destroyed enemies
    spawnedEnemies.erase(std::remove_if(spawnedEnemies.begin(), spawnedEnemies.end(),
                                        [](Entity* enemy) { return !enemy->isActive(); }),
                         spawnedEnemies.end());

    Uint32 currentTime = SDL_GetTicks();
    Vector2D spawnerPosition = spawnerTransform->position;
    Vector2D playerPosition = playerTransform->position;
    float distanceToPlayer = (spawnerPosition - playerPosition).magnitude();

    // Check if it's time to spawn again
    if (hasSpawned && currentTime - lastSpawnTime > spawnInterval) {
        hasSpawned = false; // Reset the flag to allow another spawn
    }

    // If there are fewer than 4 spawned enemies alive and the player is within 500 units
    if (!hasSpawned && spawnedEnemies.size() < 4 && distanceToPlayer < 500) {
        spawnEnemy();
        lastSpawnTime = currentTime;
        hasSpawned = true;
    }
}

void SpawnerComponent::spawnEnemy() {

    Vector2D spawnLocation = spawnerTransform->position;

    // Spawn an enemy at the spawner's location
    Entity& enemy = manager.addEntity();
    enemy.addComponent<TransformComponent>(spawnLocation.x, spawnLocation.y, 128, 128, 1);
    enemy.addComponent<SpriteComponent>(true, "enemy");
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(1,500,200,1200,60,playerTransform, playerStats, &enemyStats); //To be changed later on
    enemy.addGroup(Game::groupEnemies);

    spawnedEnemies.push_back(&enemy);

}
