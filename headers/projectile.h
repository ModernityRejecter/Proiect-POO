#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <iostream>
#include "../headers/entity.h"
class Entity;
class Projectile {
private:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float lifetime;
    static constexpr float maxLifetime = 3.0f;

    int damage;
    std::weak_ptr<Entity> owner;
    bool active;

public:
    Projectile(const sf::Texture& texture,
               float startX,
               float startY,
               float targetX,
               float targetY,
               float speed,
               float spreadAngle,
               int damage,
               std::weak_ptr<Entity> ownerPtr);

    Projectile(const Projectile& other);
    Projectile& operator=(const Projectile& other);
    ~Projectile();

    void update(float deltaTime);
    bool isAlive() const;
    void deactivate();
    int getDamage() const;
    sf::FloatRect getBounds() const;
    std::shared_ptr<Entity> getOwner() const;
    void draw(sf::RenderWindow& window) const;
};
