#pragma once
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#include <vector>
#include <random>
#include <memory>
#include <SFML/Graphics.hpp>
#include "entity.h"
#include "imp.h"
#include "player.h"

class EnemySpawner {
public:
    explicit EnemySpawner(std::vector<std::unique_ptr<Entity>>& entitiesRef);
    void init(Player* playerPtr, const sf::Vector2f& logicalSize);
    void update(float deltaTime);

private:
    enum class EnemyType {
        Imp = 0,
        COUNT
    };
    std::vector<std::unique_ptr<Entity>>& entities;
    Player* playerPtr;
    sf::Vector2f bounds;

    std::mt19937 rng;
    std::uniform_real_distribution<float> spawnIntervalDist;
    std::uniform_int_distribution<int> enemyTypeDist{0, static_cast<int>(EnemyType::COUNT) - 1};
    std::uniform_int_distribution<int> borderDist;

    float nextSpawnTime;
    float elapsedSinceLast;

    void scheduleNextSpawn();
    void spawnEnemy();
};
