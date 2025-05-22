#pragma once

#include "../headers/entity.h"
#include "../headers/player.h"
#include "../headers/projectile.h"
#include <memory>
#include <vector>
#include <unordered_map>

class Imp : public Entity, public std::enable_shared_from_this<Imp> {
private:
    std::weak_ptr<Player> targetPlayer;
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;
    sf::Texture projectileTexture;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Clock shootClock;
    float shootInterval;
    float shootRange;

    void idleAnimation() override;
    void tryToShoot();

public:
    Imp(sf::Vector2f position, const std::shared_ptr<Player> &playerPtr);
    ~Imp() override = default;
    std::vector<std::unique_ptr<Projectile>>& getProjectiles() override;
    void move(float deltaTime) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    static void loadEntityTextures();
};
