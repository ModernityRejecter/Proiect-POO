#include "../headers/pickupSpawner.h"

template class PickupSpawnerTpl<HealthPickup>;
template class PickupSpawnerTpl<ArmorPickup>;
template class PickupSpawnerTpl<AmmoPickup>;

template<typename P>
PickupSpawnerTpl<P>::PickupSpawnerTpl(std::vector<std::unique_ptr<Pickup>> &outList, float chance): spawnChance(chance), list(outList) {}

template<typename P>
void PickupSpawnerTpl<P>::trySpawn(const sf::Vector2f &pos) {
    if (chanceDist(rng) > spawnChance) return;
    list.push_back(std::make_unique<P>(pos));
}
