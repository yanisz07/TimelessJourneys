#include "SpawnerComponent.hpp"
#include "SDL_timer.h"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "EnemyMovement.hpp"
#include "armor.hpp"


SpawnerComponent::SpawnerComponent(Manager& m, Uint32 interval, int max_enemies, TransformComponent* playerTrans, Stats* playerstats, ColliderComponent* playerCol, Armor* playerArmor)
    : manager(m), spawnInterval(interval), max_spawn(max_enemies), playerTransform(playerTrans), playerStats(playerstats), hasSpawned(false) {
    lastSpawnTime = SDL_GetTicks();
    playerCollider = playerCol;
    this->playerArmor = playerArmor;
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

    // If there are fewer than 4 spawned enemies alive and the player is within X units
    if (!hasSpawned && spawnedEnemies.size() < max_spawn && distanceToPlayer < 600) {
        spawnEnemy();
        lastSpawnTime = currentTime;
        hasSpawned = true;
    }
}

void SpawnerComponent::spawnEnemy() {

    int enemy_type = (rand() % 3) + 1;

    if (enemy_type == 3) {
        enemy_type = 1; // Change a 3 to a 2 to make the chances of getting a 2 two-thirds.
    } // 1/3 creeper and 2/3 swordsman



    Vector2D spawnLocation = spawnerTransform->position;

    // Spawn an enemy at the spawner's location
    Entity& enemy = manager.addEntity();
    enemy.addComponent<TransformComponent>(spawnLocation.x, spawnLocation.y, 128, 128, 1);
    if (enemy_type ==2)
    {
        std::cout << "creeper created" << std::endl;
        enemy.addComponent<SpriteComponent>(true, "enemy_creeper");
    }
    if (enemy_type ==1)
    {
        enemy.addComponent<SpriteComponent>(true, "enemy");
    }
    enemy.getComponent<SpriteComponent>().setActions();
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addComponent<Stats>();
    Stats& enemyStats = enemy.getComponent<Stats>();
    enemy.addComponent<EnemyMovement>(enemy_type,500,200,1200,60,3,playerTransform, playerStats, &enemyStats, playerCollider, playerArmor);
    enemy.addGroup(Game::groupEnemies);

    spawnedEnemies.push_back(&enemy);

}
