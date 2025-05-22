#pragma once

#include <SFML/Graphics.hpp>
#include "../headers/projectile.h"
#include "../headers/drawable.h"
#include <vector>
#include <memory>

static constexpr float LOGICAL_WIDTH  = 1920;
static constexpr float LOGICAL_HEIGHT = 1080;

class Projectile;

class Entity : public Drawable {
protected:
    const std::string texturePath;
    const int maxHealth;
    int health;
    float speed;
    int directionIndex;
    int previousDirectionIndex;
    sf::Vector2f aimPosition;
    sf::Clock interval;
    int textureIndex;

    explicit Entity(const std::string& texturePath,
                    int maxHealth = 100,
                    int health    = 100,
                    float speed   = 100.0f,
                    sf::Vector2f position = {600, 600});
    virtual ~Entity() override = default;
    virtual int findDirection();
    virtual void move(float deltaTime);
    virtual void idleAnimation() = 0;

public:
    virtual void takeDamage(int amount);
    bool isAlive() const;
    int getHealth() const;
    sf::FloatRect getBounds() const;
    virtual void update(float deltaTime) = 0;
    virtual std::vector<std::unique_ptr<Projectile>>& getProjectiles();
    // [[maybe_unused]] virtual void shooting(const sf::Vector2f& targetPosition);
};
