#pragma once
#include "../headers/player.h"
#include "../headers/entity.h"

class Enemy : public Entity, public std::enable_shared_from_this<Enemy> {
protected:
    std::weak_ptr<Player> targetPlayer;
    void tryToShoot();
    void move(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Texture projectileTexture;
    sf::Clock shootClock;
    float shootInterval;
    float shootRange;

public:
    explicit Enemy(const std::string& texturePath,
                   int maxHealth,
                   float speed,
                   sf::Vector2f position,
                   const std::shared_ptr<Player> &targetPlayer);
    ~Enemy() override = default;
    std::vector<std::unique_ptr<Projectile>>& getProjectiles() override;

};
