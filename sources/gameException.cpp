#include <utility>

#include "../headers/gameException.h"

GameException::GameException(std::string msg): text(std::move(msg)) {}

const char * GameException::what() const noexcept {
    return text.c_str();
}

TextureLoadException::TextureLoadException(const std::string &path): GameException("Failed to load texture: " + path) {}

SoundLoadException::SoundLoadException(const std::string &path): GameException("Failed to load sound: " + path) {}

EntityVectorException::EntityVectorException(const std::string& path): GameException("Invalid amount of entities: " + path) {}

ProjectileException::ProjectileException(const std::string &msg): GameException("Projectile error: " + msg) {}

WeaponException::WeaponException(const std::string &msg): GameException("Weapon error: " + msg) {}
