#include "../headers/pinky.h"
#include <iostream>

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Pinky::entityTextures;
int Pinky::updateTime = 75;
const int Pinky::dashDamage = 80;
const float Pinky::dashHoldTime = 2.f;
const float Pinky::dashTotalDistance = 1000.f;
const float Pinky::dashCooldownTime = 3.f;
const float Pinky::detectionRange = 500.f;

Pinky::Pinky(const sf::Vector2f& position,
             const std::shared_ptr<Player>& targetPlayer)
    : Enemy("./assets/textures/enemies/pinky/sprite_1_1.png",
            140,
            250.f,
            position,
            2.f,
            50.f,
            50.f,
            1000.f,
            0,
            0,
            targetPlayer)
{
    std::string path = "./assets/textures/projectiles/pinky_proj.png";
    if (!projectileTexture.loadFromFile(path)) {
        throw TextureLoadException(path);
    }
    sprite.setPosition(position);
}

void Pinky::loadEntityTextures() {
    std::string basePath = "./assets/textures/enemies/pinky/";
    std::string prefix = "sprite_";
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 2; ++j) {
            sf::Texture tex;
            std::ignore = tex.loadFromFile(basePath + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = tex;
        }
    }
}

void Pinky::startCharging() {
    dashState = DashState::Charging;
    stateClock.restart();
}

void Pinky::startDash() {
    dashState = DashState::Dashing;
    dashHitPlayer = false;
    if (auto playerShared = targetPlayer.lock()) {
        sf::Vector2f enemyCenter = { sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.f,
                                     sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.f };
        sf::Vector2f playerCenter = { playerShared->getPosition().x + playerShared->getBounds().size.x / 2.f,
                                      playerShared->getPosition().y + playerShared->getBounds().size.y / 2.f };
        sf::Vector2f diff = playerCenter - enemyCenter;
        float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        dashDirection = (len > 0.f ? diff / len : sf::Vector2f{0.f, 0.f});
    }
    dashDistanceTraveled = 0.f;
    stateClock.restart();
}

void Pinky::finishDash() {
    dashState = DashState::Cooldown;
    stateClock.restart();
}

void Pinky::processDash(float deltaTime) {
    float step = speed * 3.f * deltaTime;
    sprite.move(dashDirection * step);
    dashDistanceTraveled += step;

    if (!dashHitPlayer) {
        if (auto playerShared = targetPlayer.lock()) {
            if (sprite.getGlobalBounds().findIntersection(playerShared->getBounds())) {
                playerShared->takeDamage(dashDamage);
                dashHitPlayer = true;
            }
        }
    }

    aimPosition = sprite.getPosition() + dashDirection;
    findDirection();
    idleAnimation(entityTextures, updateTime);

    if (dashDistanceTraveled >= dashTotalDistance) {
        finishDash();
    }
}

void Pinky::update(float deltaTime) {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    switch (dashState) {
        case DashState::Idle: {
            sf::Vector2f enemyPos = sprite.getPosition();
            sf::Vector2f playerPos = playerShared->getPosition();
            float dx = playerPos.x - enemyPos.x;
            float dy = playerPos.y - enemyPos.y;
            float dist = std::sqrt(dx*dx + dy*dy);
            if (dist <= detectionRange) {
                startCharging();
                return;
            }
            move(deltaTime);
            aimPosition = playerPos;
            findDirection();
            idleAnimation(entityTextures, updateTime);
            tryToShoot();
            break;
        }
        case DashState::Charging:
            if (stateClock.getElapsedTime().asSeconds() >= dashHoldTime) {
                startDash();
            }
            return;
        case DashState::Dashing:
            processDash(deltaTime);
            return;
        case DashState::Cooldown:
            if (stateClock.getElapsedTime().asSeconds() >= dashCooldownTime) {
                dashState = DashState::Idle;
            }
            move(deltaTime);
            aimPosition = playerShared->getPosition();
            findDirection();
            idleAnimation(entityTextures, updateTime);
            tryToShoot();
            break;
    }

    for (auto& p : projectiles) p->update(deltaTime);
    std::erase_if(projectiles, [](auto& p){ return !p->isAlive(); });
}

void Pinky::tryToShoot() {
    if (dashState == DashState::Idle || dashState == DashState::Cooldown) {
        Enemy::tryToShoot();
    }
}

std::unique_ptr<Entity> Pinky::clone() const {
    return std::make_unique<Pinky>(*this);
}
