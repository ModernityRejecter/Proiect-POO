#include "../headers/enemy.h"

Enemy::Enemy(const std::string& texturePath,
             int maxHealth,
             float speed,
             sf::Vector2f position,
             float shootInterval,
             float shootRange,
             int projectileDamage,
             float projectileSpeed,
             int armor,
             int damageReduction,
             const std::shared_ptr<Player> &targetPlayer,
             const std::string& soundPath,
             const std::string& attackSoundPath)
    : Entity(texturePath,
             soundPath,
             maxHealth,
             maxHealth,
             speed,
             armor,
             damageReduction,
             position),
             targetPlayer(targetPlayer),
             shootInterval(shootInterval),
             shootRange(shootRange),
             projectileDamage(projectileDamage),
             projectileSpeed(projectileSpeed),
             attackSoundBuffer(attackSoundPath),
             attackSound(attackSoundBuffer)
{
    attackSound.setVolume(20);
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

    sf::Vector2f enemyPos = sprite.getPosition();
    sf::Vector2f playerPos = playerShared->getPosition();

    float dx = playerPos.x - enemyPos.x;
    float dy = playerPos.y - enemyPos.y;
    float dist = std::sqrt(dx*dx + dy*dy);

    if (dist <= shootRange && shootClock.getElapsedTime().asSeconds() >= shootInterval) {
        sf::Vector2f enemyCenter = {
            enemyPos.x + sprite.getGlobalBounds().size.x / 2.f,
            enemyPos.y + sprite.getGlobalBounds().size.y / 2.f
        };
        sf::Vector2f playerCenter = {
            playerPos.x + playerShared->getBounds().size.x / 2.f,
            playerPos.y + playerShared->getBounds().size.y / 2.f
        };

        attackSound.play();
        projectiles.emplace_back(std::make_shared<Projectile>(
            projectileTexture,
            enemyCenter.x, enemyCenter.y,
            playerCenter.x, playerCenter.y,
            projectileSpeed,
            20.f,
            projectileDamage,
            shared_from_this()
        ));
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

std::vector<std::shared_ptr<Projectile>>& Enemy::getProjectiles() {
    return projectiles;
}
