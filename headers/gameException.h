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

class EntityVectorException : public GameException {
public:
    explicit EntityVectorException(const std::string& path);
};

class StateTransitionException : public GameException {
public:
    explicit StateTransitionException(const std::string& msg);
};

class WeaponException : public GameException {
public:
    explicit WeaponException(const std::string& msg);
};
