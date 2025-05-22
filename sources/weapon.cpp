#include "../headers/weapon.h"

Weapon::Weapon(const std::string& name,
               const std::string& projectilePath,
               float speed,
               float rate,
               int ammo,
               int maxAmmo,
               float spread,
               const std::string& soundPath,
               int damage)
    : name(name),
      projectileTexture(projectilePath),
      fireRate(rate),
      ammoCount(ammo),
      maxAmmo(maxAmmo),
      projectileSpeed(speed),
      spread(spread),
      soundBuffer(soundPath),
      sound(soundBuffer),
      damageValue(damage)
{
    sound.setVolume(10.f);
}

bool Weapon::canShoot() const {
    return (fireClock.getElapsedTime().asSeconds() >= 1.0f / fireRate && ammoCount > 0);
}

Projectile Weapon::createProjectile(float startX,
                                    float startY,
                                    float targetX,
                                    float targetY,
                                    const std::shared_ptr<Entity> &ownerPtr)
{
    ammoCount -= 1;
    return Projectile(projectileTexture,
                      startX,
                      startY,
                      targetX,
                      targetY,
                      projectileSpeed,
                      spread,
                      damageValue,
                      ownerPtr);
}

std::ostream& operator<<(std::ostream& info, const Weapon& weapon) {
    info << "Weapon name: " << weapon.name << "\n"
         << "Fire rate: " << weapon.fireRate << "\n"
         << "Projectile speed: " << weapon.projectileSpeed << "\n"
         << "Max ammo: " << weapon.getMaxAmmo();
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

void Weapon::resetFireClock() {
    fireClock.restart();
}

void Weapon::playSound() {
    sound.play();
}

float Weapon::getProjectileSpeed() const {
    return projectileSpeed;
}

float Weapon::getSpreadAngle() const {
    return spread;
}

int Weapon::getDamageValue() const {
    return damageValue;
}
