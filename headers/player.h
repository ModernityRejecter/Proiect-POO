#pragma once
#include <SFML/Graphics.hpp>
#include "../headers/weapon.h"
#include "../headers/entity.h"
class Player : public Entity{
private:

    sf::Clock shotClock;
    bool isShooting = false;
    std::vector<Weapon> weapons;
    std::vector<Projectile> playerProjectiles;
    size_t currentWeaponIndex = 0;
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;
    int armor = 200;

    void move(float deltaTime) override;
    void loadWeaponsAttributes();
    void weaponsHandler();
    void playerShooting();
    void idleAnimation() override;
    void shootingAnimation();
    void updateShootingAnimation();
public:
    Player(const std::string& texturePath, float x, float y, float speed);
    friend std::ostream& operator<<(std::ostream& info, const Player& player);
    static void loadEntityTextures();
    void draw(sf::RenderWindow& window) const override;
    // size_t getWeaponIndex() const;
    int getWeaponAmmoCount() const;
    // int getWeaponMaxAmmo() const;
    ~Player() override = default;
    void setAim(const sf::Vector2f& worldMousePos);
    int getWeaponMaxAmmo() const;
    int getPlayerArmor() const;
    void update(float deltaTime) override;
};