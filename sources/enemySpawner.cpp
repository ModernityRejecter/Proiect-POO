#include "../headers/enemySpawner.h"

EnemySpawner::EnemySpawner(std::vector<std::shared_ptr<Entity>>& entitiesRef)
    : entities(entitiesRef),
      bounds({0.f, 0.f}),
      rng(std::random_device{}()),
      spawnIntervalDist(0.0f, 3.0f),
      enemyTypeDist(0, static_cast<int>(EnemyType::COUNT) - 1),
      borderDist(0, 3),
      nextSpawnTime(0.f),
      elapsedSinceLast(0.f)
{}

void EnemySpawner::init(const std::shared_ptr<Player> &playerPtr_, const sf::Vector2f& logicalSize) {
    playerPtr = playerPtr_;
    bounds = logicalSize;
    scheduleNextSpawn();
}

void EnemySpawner::scheduleNextSpawn() {
    nextSpawnTime = spawnIntervalDist(rng);
    elapsedSinceLast = 0.f;
}

void EnemySpawner::update(float deltaTime) {
    auto playerShared = playerPtr.lock();
    if (!playerShared) return;

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
            y = bounds.y - 64;
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
        default: ;
    }

    switch (enemyTypeDist(rng)) {
        case static_cast<int>(EnemyType::Imp):
            entities.push_back(std::make_shared<Imp>(
                sf::Vector2f{x, y},
                std::dynamic_pointer_cast<Player>(playerPtr.lock())
            ));
        break;

        case static_cast<int>(EnemyType::CyberDemon):
            entities.push_back(std::make_shared<CyberDemon>(
                sf::Vector2f{x, y},
                std::dynamic_pointer_cast<Player>(playerPtr.lock())
            ));
        break;

        case static_cast<int>(EnemyType::Pinky):
            entities.push_back(std::make_shared<Pinky>(
                sf::Vector2f{x, y},
                std::dynamic_pointer_cast<Player>(playerPtr.lock())
            ));
        break;
        default: ;
    }
}
