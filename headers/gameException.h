#pragma once

#include <exception>
#include <string>


class GameException : public std::exception {
protected:
    std::string text;
public:
    explicit GameException(std::string msg);

    [[nodiscard]] const char* what() const noexcept override;
};

class TextureLoadException : public GameException {
public:
    explicit TextureLoadException(const std::string& path);
};

class SoundLoadException : public GameException {
public:
    explicit SoundLoadException(const std::string& path);
};

class EntityStateException : public GameException {
public:
    explicit EntityStateException(const std::string& msg);
};

class ProjectileException : public GameException {
public:
    explicit ProjectileException(const std::string& msg);
};

class WeaponException : public GameException {
public:
    explicit WeaponException(const std::string& msg);
};
