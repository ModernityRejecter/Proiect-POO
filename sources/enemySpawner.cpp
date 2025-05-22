#include "../headers/enemySpawner.h"

EnemySpawner::EnemySpawner(std::vector<std::unique_ptr<Entity>>& entitiesRef)
    : entities(entitiesRef),
      playerPtr(nullptr),
      bounds({0.f, 0.f}),
      rng(std::random_device{}()),
      spawnIntervalDist(2.0f, 5.0f),
      borderDist(0, 3),
      nextSpawnTime(0.f),
      elapsedSinceLast(0.f)
{

}

void EnemySpawner::init(Player* p, const sf::Vector2f& logicalSize) {
    playerPtr = p;
    bounds = logicalSize;
    scheduleNextSpawn();
}

void EnemySpawner::scheduleNextSpawn() {
    nextSpawnTime = spawnIntervalDist(rng);
    elapsedSinceLast = 0.f;
}

void EnemySpawner::update(float deltaTime) {
    if (!playerPtr) {
        return;
    }

    elapsedSinceLast += deltaTime;
    if (elapsedSinceLast >= nextSpawnTime) {
        spawnEnemy();
        scheduleNextSpawn();
    }
}

void EnemySpawner::spawnEnemy() {
    int border = borderDist(rng);

    float x = 0.f, y = 0.f;
    switch (border) {
        case 0:
            y = 0.f;
            x = std::uniform_real_distribution<float>(0.f, bounds.x)(rng);
            break;
        case 1:
            y = bounds.y;
            x = std::uniform_real_distribution<float>(0.f, bounds.x)(rng);
            break;
        case 2:
            x = 0.f;
            y = std::uniform_real_distribution<float>(0.f, bounds.y)(rng);
            break;
        case 3:
            x = bounds.x;
            y = std::uniform_real_distribution<float>(0.f, bounds.y)(rng);
            break;
        default:
            break;
    }

    switch (int typeIndex = enemyTypeDist(rng); static_cast<EnemyType>(typeIndex)) {
        case EnemyType::Imp: {
            entities.push_back(std::make_unique<Imp>(
                sf::Vector2f{x, y},
                playerPtr
            ));
            break;
        }
        default:
            break;
    }
}
