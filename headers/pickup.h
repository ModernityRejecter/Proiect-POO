#pragma once

#include <SFML/Graphics.hpp>
#include "../headers/drawable.h"
#include "../headers/player.h"
#include <memory>

enum class PickupType { Health, Armor, Ammo };
class Player;

class Pickup : public Drawable {
public:
    explicit Pickup(const std::string& texPath, const sf::Vector2f& pos)
        : Drawable(texPath, pos) {}

    ~Pickup() override = default;
    virtual void apply(Player& player) = 0;
};

class HealthPickup : public Pickup {
public:
    explicit HealthPickup(const sf::Vector2f& pos);
    void apply(Player& player) override;
};

class ArmorPickup : public Pickup {
public:
    explicit ArmorPickup(const sf::Vector2f& pos);
    void apply(Player& player) override;
};

class AmmoPickup : public Pickup {
public:
    explicit AmmoPickup(const sf::Vector2f& pos);
    void apply(Player& player) override;
};

class PickupFactory {
public:
    template<typename T>
    static std::unique_ptr<Pickup> create(const sf::Vector2f& pos) {
        return std::make_unique<T>(pos);
    }
    static std::unique_ptr<Pickup> create(PickupType type,
                                          const sf::Vector2f& pos);
};
