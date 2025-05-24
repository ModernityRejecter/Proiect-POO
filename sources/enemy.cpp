#include "../headers/Enemy.h"

Enemy::Enemy(const std::string& texturePath,
             int maxHealth,
             float speed,
             sf::Vector2f position, const std::shared_ptr<Player> &targetPlayer)
    : Entity(texturePath,
             maxHealth,
             maxHealth,
             speed,
             position),
             targetPlayer(targetPlayer),
             shootInterval(0.5f),
             shootRange(800.f)
{
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    for (const auto& p : projectiles) {
        p->draw(window);
    }
}

void Enemy::tryToShoot() {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    sf::Vector2f impPos = sprite.getPosition();
    sf::Vector2f playerPos = playerShared->getPosition();

    float dx = playerPos.x - impPos.x;
    float dy = playerPos.y - impPos.y;
    float dist = std::sqrt(dx*dx + dy*dy);

    if (dist <= shootRange && shootClock.getElapsedTime().asSeconds() >= shootInterval) {
        sf::Vector2f impCenter = {
            impPos.x + sprite.getGlobalBounds().size.x  / 2.f,
            impPos.y + sprite.getGlobalBounds().size.y / 2.f
        };
        sf::Vector2f playerCenter = {
            playerPos.x + playerShared->getBounds().size.x  / 2.f,
            playerPos.y + playerShared->getBounds().size.y / 2.f
        };

        auto p = std::make_unique<Projectile>(
            projectileTexture,
            impCenter.x, impCenter.y,
            playerCenter.x, playerCenter.y,
            1000.f,
            20.f,
            5,
            shared_from_this()
        );
        projectiles.push_back(std::move(p));
        shootClock.restart();
    }
}

void Enemy::move(float deltaTime) {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    sf::Vector2f zombiePos = sprite.getPosition();
    sf::Vector2f playerPos = playerShared->getPosition();

    sf::Vector2f diff = playerPos - zombiePos;
    float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y);

    if (dist > 0.0f) {
        sf::Vector2f dir = diff / dist;
        sprite.move(dir * speed * deltaTime);
        position = sprite.getPosition();
    }
}

std::vector<std::unique_ptr<Projectile>>& Enemy::getProjectiles() {
    return projectiles;
}