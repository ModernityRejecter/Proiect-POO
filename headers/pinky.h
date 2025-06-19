#pragma once

#include <SFML/Graphics.hpp>
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
    enum class DashState { Idle, Charging, Dashing, Cooldown };
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;
    static int updateTime;
    DashState dashState = DashState::Idle;
    sf::Clock stateClock;
    sf::Vector2f dashDirection;
    float dashDistanceTraveled = 0.f;
    bool dashHitPlayer = false;

    static const float dashHoldTime;
    static const float dashTotalDistance;
    static const float dashCooldownTime;
    static const float detectionRange;
    static const int dashDamage;

    void startCharging();
    void startDash();
    void finishDash();
    void processDash(float deltaTime);
    void tryToShoot() override;

public:
    Pinky(const sf::Vector2f& position,
          const std::shared_ptr<Player>& playerPtr,
          const std::string& soundPath,
          const std::string& attackSoundPath);
    ~Pinky() override = default;
    void update(float deltaTime) override;
    std::unique_ptr<Entity> clone() const override;
    static void loadEntityTextures();
};