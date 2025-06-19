#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    int armor;
    int damageReduction;
    sf::SoundBuffer painSoundBuffer;
    sf::Sound painSound;

    explicit Entity(const std::string& texturePath,
                    const std::string& soundPath,
                    int maxHealth = 100,
                    int health = 100,
                    float speed = 100.0f,
                    int armor = 0,
                    int damageReduction = 0,
                    sf::Vector2f position = {600, 600});
    virtual int findDirection();
    virtual void move(float deltaTime);
    virtual void idleAnimation(std::unordered_map<int, std::unordered_map<int, sf::Texture>>& entityTextures, int updateTime);

public:
    ~Entity() override = default;
    virtual void takeDamage(int amount);
    bool isAlive() const;
    int getHealth() const;
    void setHealth(const int& newHealth);
    void setArmor(const int& newArmor);
    virtual void update(float deltaTime) = 0;
    virtual void playPainSound();
    virtual std::vector<std::shared_ptr<Projectile>>& getProjectiles();
};
