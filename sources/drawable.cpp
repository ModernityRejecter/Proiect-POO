#include "../headers/drawable.h"

Drawable::Drawable(const std::string& texturePath, const sf::Vector2f& position) : texture(texturePath), sprite(texture), position(position) {
    // std::cout<<"Drawable created at position ("<<position.x<<", "<<position.y<<")"<<std::endl;
    sprite.setPosition(position);
    if (!texture.loadFromFile(texturePath)) {
        throw TextureLoadException(texturePath);
    }
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

void Drawable::display(sf::RenderWindow& window) const {
    draw(window);
}

sf::FloatRect Drawable::getBounds() const {
    return sprite.getGlobalBounds();
}