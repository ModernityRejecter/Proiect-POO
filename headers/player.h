#pragma once
#include <SFML/Graphics.hpp>
#include "../headers/weapon.h"
class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    sf::Clock interval;
    std::unordered_map<int, std::unordered_map<int, sf::Texture>> playerTextures;
    int textureIndex;
    int directionIndex;
    int previousDirectionIndex;
    std::vector<Weapon> weapons;
    std::vector<Projectile> playerProjectiles;
    size_t currentWeaponIndex = 0;

public:
    Player(const std::string& texturePath, float x, float y, float speed);
    friend std::ostream& operator<<(std::ostream& info, const Player& player);
    void loadPlayerTextures();
    void move(float offsetX, float offsetY);
    void shootingAnimation();
    void loadWeaponsAttributes();
    void idleAnimation();
    void update(float deltaTime, unsigned int width, unsigned int height);
    void weaponsHandler();
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    sf::Vector2u getSize() const;
    // size_t getWeaponIndex() const;
    int getWeaponAmmoCount() const;
    // int getWeaponMaxAmmo() const;
};