#pragma once
#include "../headers/entity.h"
#include "../headers/player.h"
#include "../headers/projectile.h"

class Imp : public Entity {
private:
    Player* targetPlayer;
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;

    sf::Texture projectileTexture;
    std::vector<Projectile> impProjectiles;

    sf::Clock shootClock;
    float shootInterval = 1.5f;
    float shootRange = 400.f;

    void idleAnimation() override;
    void tryToShoot();

public:
    Imp(sf::Vector2f position, Player* playerPtr);
    void move(float deltaTime) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;

    static void loadEntityTextures();
};
