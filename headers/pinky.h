#pragma once

#include "../headers/entity.h"
#include "../headers/player.h"
#include "../headers/projectile.h"
#include "../headers/enemy.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>

class Pinky : public Enemy {
private:
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;
    static int updateTime;

public:
    Pinky(const sf::Vector2f& position, const std::shared_ptr<Player> &playerPtr);
    ~Pinky() override = default;
    std::unique_ptr<Entity> clone() const override;
    static void loadEntityTextures();
    void update(float deltaTime) override;
};