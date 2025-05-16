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
    sf::Vector2f aimPosition;
    int armor = 200;

    void move(float deltaTime, unsigned int width, unsigned int height) override;
    void loadWeaponsAttributes();
    void weaponsHandler();
    void playerShooting();
    void idleAnimation();
    void shootingAnimation();

public:
    Player(const std::string& texturePath, float x, float y, float speed);
    friend std::ostream& operator<<(std::ostream& info, const Player& player);
    void loadEntityTextures() override;
    void draw(sf::RenderWindow& window) const override;
    // size_t getWeaponIndex() const;
    int getWeaponAmmoCount() const;
    // int getWeaponMaxAmmo() const;
    ~Player() override = default;
    void setAim(const sf::Vector2f& worldMousePos);
    int getWeaponMaxAmmo() const;
    int getPlayerArmor() const;
    void update(float deltaTime, unsigned int width, unsigned int height) override;
};