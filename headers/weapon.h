#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/projectile.h"
#include "../headers/entity.h"
#include <string>
#include <iostream>
#include <memory>



class Weapon {
private:
    std::string name;
    sf::Texture projectileTexture;
    float fireRate;
    int ammoCount;
    const int maxAmmo;
    const float projectileSpeed;
    sf::Clock fireClock;
    float spread;
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;
    int damageValue;

public:
    Weapon(const std::string& name,
           const std::string& projectilePath,
           float speed,
           float rate,
           int ammo,
           int maxAmmo,
           float spread,
           const std::string& soundPath,
           int damage);

    bool canShoot() const;
    Projectile createProjectile( float startX,
                                 float startY,
                                 float targetX,
                                 float targetY,
                                 const std::shared_ptr<Entity> &ownerPtr);

    friend std::ostream& operator<<(std::ostream& info, const Weapon& weapon);
    float getFireRate() const;
    int getAmmoCount() const;
    int getMaxAmmo() const;
    void resetFireClock();
    void playSound();
    void addAmmo();

    // float getProjectileSpeed() const;
    // float getSpreadAngle() const;
    // int getDamageValue() const;
};
