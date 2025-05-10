#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
class Drawable {
protected:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;

public:
    Drawable(const sf::Vector2f& position, const std::string& texturePath);

    void setPosition(sf::Vector2f pos);

    virtual void draw(sf::RenderWindow& window) const;

    sf::Vector2u getSize(sf::RenderWindow& window) const;

    virtual ~Drawable() = default;
};
