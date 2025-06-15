#include "../headers/pickup.h"

HealthPickup::HealthPickup(const sf::Vector2f& pos)
    : Pickup("./assets/textures/pickups/health.png", pos){}
void HealthPickup::apply(Player& player) {
    player.setHealth(player.getHealth() + 10);
}

ArmorPickup::ArmorPickup(const sf::Vector2f& pos)
    : Pickup("./assets/textures/pickups/armor.png", pos) {}
void ArmorPickup::apply(Player& player) {
    player.setArmor(player.getPlayerArmor() + 20);
}

AmmoPickup::AmmoPickup(const sf::Vector2f& pos)
    : Pickup("./assets/textures/pickups/ammo.png", pos){}
void AmmoPickup::apply(Player& player) {
    player.addAmmo();
}

std::unique_ptr<Pickup> PickupFactory::create(PickupType type,
                                              const sf::Vector2f& pos) {
    switch(type) {
        case PickupType::Health: return create<HealthPickup>(pos);
        case PickupType::Armor: return create<ArmorPickup>(pos);
        case PickupType::Ammo: return create<AmmoPickup>(pos);
    }
    return nullptr;
}
