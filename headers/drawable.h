#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
class Drawable {
protected:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;

public:
    Drawable(const std::string& texturePath, const sf::Vector2f& position = {600, 600});

    virtual void setPosition(sf::Vector2f pos);

    virtual void draw(sf::RenderWindow& window) const;

    virtual sf::Vector2u getSize() const;

    virtual ~Drawable() = default;
};
