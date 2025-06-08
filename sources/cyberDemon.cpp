#include "../headers/cyberDemon.h"
#include <iostream>

std::unordered_map<int, std::unordered_map<int, sf::Texture>> CyberDemon::entityTextures;
int CyberDemon::updateTime = 200;
const float CyberDemon::detectionRange = 250.f;
const float CyberDemon::chargeTime = 1.f;
const float CyberDemon::cooldownTime = 10.f;
const float CyberDemon::angleOffsetDeg = 15.f;

CyberDemon::CyberDemon(const sf::Vector2f& position,
                       const std::shared_ptr<Player>& targetPlayer)
    : Enemy("./assets/textures/enemies/cyber_demon/sprite_1_1.png",
            400,
            60.f,
            position,
            2.2f,
            500.f,
            30,
            400.f,
            0,
            0,
            targetPlayer)
{
    std::string path = "./assets/textures/projectiles/cyber_demon_proj.png";
    if (!projectileTexture.loadFromFile(path)) {
        throw TextureLoadException(path);
    }
    sprite.setPosition(position);
}

void CyberDemon::loadEntityTextures() {
    std::string basePath = "./assets/textures/enemies/cyber_demon/";
    std::string prefix = "sprite_";
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 2; ++j) {
            sf::Texture tex;
            std::ignore = tex.loadFromFile(basePath + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = tex;
        }
    }
}

void CyberDemon::startCharging() {
    abilityState = AbilityState::Charging;
    stateClock.restart();
}

void CyberDemon::performAbility() {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;
    sf::Vector2f enemyCenter = { sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.f,
                                 sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.f };
    sf::Vector2f playerCenter = { playerShared->getPosition().x + playerShared->getBounds().size.x / 2.f,
                                  playerShared->getPosition().y + playerShared->getBounds().size.y / 2.f };
    sf::Vector2f diff = playerCenter - enemyCenter;
    float baseAngle = std::atan2(diff.y, diff.x);

    std::array<float, 3> angles = { baseAngle,
                                   baseAngle + angleOffsetDeg * 3.14159265f/180.f,
                                   baseAngle - angleOffsetDeg * 3.14159265f/180.f };

    for (float ang : angles) {
        sf::Vector2f dir{ std::cos(ang), std::sin(ang) };
        projectiles.emplace_back(std::make_shared<Projectile>(
            projectileTexture,
            enemyCenter.x, enemyCenter.y,
            enemyCenter.x + dir.x * 1000.f,
            enemyCenter.y + dir.y * 1000.f,
            projectileSpeed,
            20.f,
            projectileDamage,
            shared_from_this()
        ));
    }
    abilityState = AbilityState::Cooldown;
    stateClock.restart();
}

void CyberDemon::update(float deltaTime) {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    switch (abilityState) {
        case AbilityState::Idle: {
            sf::Vector2f enemyPos = sprite.getPosition();
            sf::Vector2f playerPos = playerShared->getPosition();
            float dx = playerPos.x - enemyPos.x;
            float dy = playerPos.y - enemyPos.y;
            float dist = std::sqrt(dx*dx + dy*dy);
            if (dist <= detectionRange) {
                startCharging();
            } else {
                move(deltaTime);
                aimPosition = playerPos;
                findDirection();
                idleAnimation(entityTextures, updateTime);
                tryToShoot();
            }
            break;
        }
        case AbilityState::Charging:
            if (stateClock.getElapsedTime().asSeconds() >= chargeTime) {
                performAbility();
            }
            break;
        case AbilityState::Cooldown:
            move(deltaTime);
            aimPosition = playerShared->getPosition();
            findDirection();
            idleAnimation(entityTextures, updateTime);
            tryToShoot();
            if (stateClock.getElapsedTime().asSeconds() >= cooldownTime) {
                abilityState = AbilityState::Idle;
            }
            break;
        default:
            break;
    }

    for (auto& p : projectiles) p->update(deltaTime);
    std::erase_if(projectiles, [](auto& p){ return !p->isAlive(); });
}

std::unique_ptr<Entity> CyberDemon::clone() const {
    return std::make_unique<CyberDemon>(*this);
}