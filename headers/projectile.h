#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
class Projectile {
private:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float lifetime;
    static constexpr float maxLifetime = 3.0f;

public:
    Projectile(const sf::Texture& texture, float startX, float startY, float targetX, float targetY, float speed, float spreadAngle);
    Projectile (const Projectile& other);
    ~Projectile();
    Projectile& operator=(const Projectile& other);
    friend std::ostream& operator<<(std::ostream& info, const Projectile& projectile);
    void update(float deltaTime);
    bool isAlive() const;
    void draw(sf::RenderWindow& window) const;
};
