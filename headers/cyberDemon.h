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

class CyberDemon : public Enemy {
private:
    enum class AbilityState { Idle, Charging, Shooting, Cooldown };
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;
    static int updateTime;

    AbilityState abilityState = AbilityState::Idle;
    sf::Clock stateClock;

    static const float detectionRange;
    static const float chargeTime;
    static const float cooldownTime;
    static const float angleOffsetDeg;

    void startCharging();
    void performAbility();

public:
    CyberDemon(const sf::Vector2f& position, const std::shared_ptr<Player>& playerPtr);
    ~CyberDemon() override = default;
    static void loadEntityTextures();
    void update(float deltaTime) override;
    std::unique_ptr<Entity> clone() const override;
};