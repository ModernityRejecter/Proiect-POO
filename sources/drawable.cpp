#include "../headers/drawable.h"

Drawable::Drawable(const sf::Vector2f& position, const std::string& texturePath) : texture(texturePath), sprite(texture), position(position) {
    std::cout<<"Drawable created at position ("<<position.x<<", "<<")"<<std::endl;
}

void Drawable::setPosition(sf::Vector2f pos) {
    position.x = pos.x;
    position.y = pos.y;
}

void Drawable::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::Vector2u Drawable::getSize(sf::RenderWindow& window) const {
    return texture.getSize();
}
