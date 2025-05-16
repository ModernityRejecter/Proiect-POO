#include "../headers/weapon.h"
Weapon::Weapon(const std::string& name, const std::string& projectilePath, float speed, float rate, int ammo, int maxAmmo, float spread)
    : name(name), fireRate(rate), ammoCount(ammo), maxAmmo(maxAmmo), projectileSpeed(speed), spread(spread) {
    if (!projectileTexture.loadFromFile(projectilePath)) {
        std::cerr << "Eroare la incarcarea texturii pentru " << name << "\n";
        exit(EXIT_FAILURE);
    }
}

bool Weapon::canShoot() const {
    return (fireClock.getElapsedTime().asSeconds() >= 1.0f / fireRate && ammoCount != 0);
}

void Weapon::resetFireClock() {
    fireClock.restart();
}

Projectile Weapon::createProjectile(float startX, float startY, float targetX, float targetY) {
    ammoCount -= 1;
    return {projectileTexture, startX, startY, targetX, targetY, projectileSpeed, spread};
}

std::ostream& operator<<(std::ostream& info, const Weapon& weapon) {
    info << "Weapon name: " << weapon.name << "\n"
         << "Fire rate: " << weapon.fireRate << "\n"
         << "Projectile speed: " << weapon.projectileSpeed << "\n"
         << "Max ammo : "<<weapon.getMaxAmmo();
    return info;
}

float Weapon::getFireRate() const {
    return fireRate;
}
int Weapon::getAmmoCount() const {
    return ammoCount;
}
int Weapon::getMaxAmmo() const {
    return maxAmmo;
}
