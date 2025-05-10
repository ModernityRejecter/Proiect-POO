#pragma once
#include <SFML/Graphics.hpp>
#include "../headers/weapon.h"
#include "../headers/entity.h"
class Player : public Entity{
private:

    sf::Clock interval;
    int textureIndex;
    std::vector<Weapon> weapons;
    std::vector<Projectile> playerProjectiles;
    size_t currentWeaponIndex = 0;

public:
    Player(const std::string& texturePath, float x, float y, float speed);
    friend std::ostream& operator<<(std::ostream& info, const Player& player);
    void loadEntityTextures() override;
    void move(float deltaTime, unsigned int width, unsigned int height) override;
    void shootingAnimation();
    void loadWeaponsAttributes();
    void idleAnimation();
    void update(float deltaTime, unsigned int width, unsigned int height) override;
    void weaponsHandler();
    void draw(sf::RenderWindow& window) const override;
    // size_t getWeaponIndex() const;
    int getWeaponAmmoCount() const;
    // int getWeaponMaxAmmo() const;
    virtual ~Player() override = default;
    void playerShooting();
};