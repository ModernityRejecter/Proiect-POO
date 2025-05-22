#pragma once

#include <vector>
#include <random>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../headers/entity.h"
#include "../headers/player.h"
#include "../headers/imp.h"

class EnemySpawner {
public:
    explicit EnemySpawner(std::vector<std::shared_ptr<Entity>>& entitiesRef);
    void init(const std::shared_ptr<Player> &playerPtr, const sf::Vector2f& logicalSize);
    void update(float deltaTime);

private:
    enum class EnemyType {
        Imp = 0,
        COUNT
    };

    std::vector<std::shared_ptr<Entity>>& entities;
    std::weak_ptr<Player> playerPtr;
    sf::Vector2f bounds;

    std::mt19937 rng;
    std::uniform_real_distribution<float> spawnIntervalDist;
    std::uniform_int_distribution<int> enemyTypeDist;
    std::uniform_int_distribution<int> borderDist;

    float nextSpawnTime;
    float elapsedSinceLast;

    void scheduleNextSpawn();
    void spawnEnemy();
};
