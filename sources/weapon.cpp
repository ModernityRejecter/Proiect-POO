#include "../headers/weapon.h"


Weapon::Weapon(const std::string& name,
               const std::string& projPath,
               float speed,
               float rate,
               int ammo,
               int maxAmmo,
               float spread,
               const std::string& soundPath,
               int damage)
    : name(name),
      projectileTexture(),
      fireRate(rate),
      ammoCount(ammo),
      maxAmmo(maxAmmo),
      projectileSpeed(speed),
      spread(spread),
      soundBuffer(),
      sound(soundBuffer),
      damageValue(damage)
{
    if (!projectileTexture.loadFromFile(projPath)) {
        throw TextureLoadException(projPath);
    }
    if (!soundBuffer.loadFromFile(soundPath)) {
        throw SoundLoadException(soundPath);
    }
    sound.setBuffer(soundBuffer);
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

void Weapon::addAmmo() {
    if (ammoCount + maxAmmo * 20 / 100 <= maxAmmo)
        ammoCount += maxAmmo * 20 / 100;
    else
        ammoCount = maxAmmo;
}