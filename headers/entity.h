#pragma once
#include <SFML/Graphics.hpp>

#include "../headers/drawable.h"

class Entity : public Drawable{
protected:
    std::string texturePath;
    int maxHealth;
    int health;
    float speed;
    std::unordered_map<int, std::unordered_map<int, sf::Texture>> playerTextures;
public:
    Entity (const std::string& texturePath, int maxHealth, int health, float speed);
    virtual void loadEntityTextures() = 0;
    virtual ~Entity() override;
    virtual void move(float offsetX, float offsetY);
    virtual void update(float deltaTime, unsigned int width, unsigned int height) = 0;
};
