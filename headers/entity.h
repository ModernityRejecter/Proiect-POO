#pragma once
#include <SFML/Graphics.hpp>

#include "../headers/drawable.h"

class Entity : public Drawable{
protected:
    const std::string texturePath;
    const int maxHealth;
    int health;
    float speed;
    int directionIndex;
    int previousDirectionIndex;
    std::unordered_map<int, std::unordered_map<int, sf::Texture>> playerTextures;

    virtual void move(float deltaTime);

public:
    Entity (const std::string& texturePath, int maxHealth = 100, int health = 100, float speed = 100.0f, sf::Vector2f  = {600,600});
    virtual void loadEntityTextures() = 0;
    virtual ~Entity() override;
    virtual void update(float deltaTime) = 0;
    [[maybe_unused]] virtual void shooting(const sf::Vector2f& targetPosition);
    int virtual getHealth() const;
};
