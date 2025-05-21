#include "../headers/drawable.h"

Drawable::Drawable(const std::string& texturePath, const sf::Vector2f& position) : texture(texturePath), sprite(texture), position(position) {
    std::cout<<"Drawable created at position ("<<position.x<<", "<<position.y<<")"<<std::endl;
    sprite.setPosition(position);
}

void Drawable::setPosition(sf::Vector2f pos) {
    position.x = pos.x;
    position.y = pos.y;
}

void Drawable::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::Vector2u Drawable::getSize() const {
    return texture.getSize();
}

sf::Vector2f Drawable::getPosition() const {
    return sprite.getPosition();
}
