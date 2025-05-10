#include "../headers/entity.h"

Entity::Entity (const std::string& texturePath, int maxHealth, int health, float speed, sf::Vector2f position) : Drawable(texturePath, position), maxHealth(maxHealth), health(health), speed(speed) {
    std::cout<<"skibidi"<<std::endl;
}
void Entity::loadEntityTextures() {

}
void Entity::move(float offsetX, float offsetY) {
    sprite.move({offsetX, offsetY});
}
Entity::~Entity() = default;

void Entity::update(float /*deltaTime*/, unsigned int /*width*/, unsigned int /*height*/) {

}