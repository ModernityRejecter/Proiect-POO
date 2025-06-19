#pragma once
#include "../headers/player.h"
#include "../headers/entity.h"
#include <SFML/Audio.hpp>

class Enemy : public Entity, public std::enable_shared_from_this<Enemy> {
protected:
    std::weak_ptr<Player> targetPlayer;

    virtual void tryToShoot();
    void move(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    std::vector<std::shared_ptr<Projectile>> projectiles;
    sf::Texture projectileTexture;
    sf::Clock shootClock;
    float shootInterval;
    float shootRange;
    int projectileDamage;
    float projectileSpeed;
    sf::SoundBuffer attackSoundBuffer;
    sf::Sound attackSound;

public:
    explicit Enemy(const std::string& texturePath,
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
                   const std::string& attackSoundPath);
    ~Enemy() override = default;
    std::vector<std::shared_ptr<Projectile>>& getProjectiles() override;
    virtual std::unique_ptr<Entity> clone() const = 0;
};
