#pragma once

#include <random>
#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp>
#include "../headers/pickup.h"

class Pickup;

template<typename P>
class PickupSpawnerTpl {
    std::mt19937 rng{ std::random_device{}()};
    std::uniform_real_distribution<float> chanceDist{ 0.f, 1.f };
    std::uniform_int_distribution<int> amtDist{ 1, 3 };
    float spawnChance;
    std::vector<std::unique_ptr<Pickup>>& list;

public:
    explicit PickupSpawnerTpl(std::vector<std::unique_ptr<Pickup>>& outList,
                     float chance = 0.3f);

    void trySpawn(const sf::Vector2f& pos);
};
