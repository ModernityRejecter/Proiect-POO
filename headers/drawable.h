#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../headers/gameException.h"

class Drawable {
protected:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;

    virtual void draw(sf::RenderWindow& window) const;
public:
    explicit Drawable(const std::string& texturePath, const sf::Vector2f& position = {600, 600});
    virtual void display(sf::RenderWindow& window) const;
    virtual sf::Vector2u getSize() const;
    virtual ~Drawable() = default;
    virtual sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
};
