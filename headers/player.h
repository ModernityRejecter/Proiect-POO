#pragma once

#include <SFML/Graphics.hpp>
#include "../headers/weapon.h"
#include "../headers/entity.h"
#include "../headers/projectile.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Player : public Entity, public std::enable_shared_from_this<Player> {
private:
    sf::Clock shotClock;
    bool isShooting;
    std::vector<Weapon> weapons;
    size_t currentWeaponIndex;
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;
    const int updateTime = 300;

    void move(float deltaTime) override;
    void loadWeaponsAttributes();
    void weaponsHandler();
    void playerShooting();
    void shootingAnimation();
    void updateShootingAnimation();

public:
    Player(const std::string& texturePath, float x, float y, float speed, const std::string& soundPath);
    ~Player() override = default;

    void draw(sf::RenderWindow& window) const override;
    int getWeaponAmmoCount() const;
    int getWeaponMaxAmmo() const;
    int getPlayerArmor() const;
    void setAim(const sf::Vector2f& worldMousePos);
    void update(float deltaTime) override;
    static void loadEntityTextures();
    void addAmmo();
    friend std::ostream& operator<<(std::ostream& info, const Player& player);
    int onDeath() override;
};
