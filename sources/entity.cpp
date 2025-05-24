#include "../headers/entity.h"

Entity::Entity(const std::string& texturePath,
               int maxHealth,
               int health,
               float speed,
               sf::Vector2f position)
    : Drawable(texturePath, position),
      texturePath(texturePath),
      maxHealth(maxHealth),
      health(health),
      speed(speed),
      directionIndex(1),
      previousDirectionIndex(1),
      textureIndex(1)
{

}

void Entity::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Entity::isAlive() const {
    return health > 0;
}

int Entity::getHealth() const {
    return health;
}

sf::FloatRect Entity::getBounds() const {
    return sprite.getGlobalBounds();
}

int Entity::findDirection() {
    float angle = std::atan2(aimPosition.y - position.y,
                             aimPosition.x - position.x) * 180.f / 3.14159265f;

    previousDirectionIndex = directionIndex;
    if (angle >= -22.5f && angle < 22.5f)
        directionIndex = 1;
    else if (angle >= 22.5f && angle < 67.5f)
        directionIndex = 2;
    else if (angle >= 67.5f && angle < 112.5f)
        directionIndex = 3;
    else if (angle >= 112.5f && angle < 157.5f)
        directionIndex = 4;
    else if (angle >= 157.5f || angle < -157.5f)
        directionIndex = 5;
    else if (angle < -112.5f)
        directionIndex = 6;
    else if (angle < -67.5f)
        directionIndex = 7;
    else if (angle < -22.5f)
        directionIndex = 8;

    return directionIndex;
}

void Entity::move(float /*deltaTime*/) {
    sf::Vector2f movement(0.f, 0.f);
    position = getPosition();

    float minX = 0.f;
    float maxX = LOGICAL_WIDTH - static_cast<float>(getSize().x);
    float minY = 0.f;
    float maxY = LOGICAL_HEIGHT - static_cast<float>(getSize().y) - 192.0f;
    sf::Vector2f newPos = position + movement;

    if (newPos.x < maxX && newPos.x > minX && newPos.y < maxY && newPos.y > minY)
        sprite.move(movement);
}

std::vector<std::shared_ptr<Projectile>>& Entity::getProjectiles() {
    static std::vector<std::shared_ptr<Projectile>> emptyList;
    return emptyList;
}

void Entity::idleAnimation(std::unordered_map<int, std::unordered_map<int, sf::Texture>>& entityTextures, int updateTime) {
    if (previousDirectionIndex != directionIndex) {
        sprite.setTexture(entityTextures[directionIndex][1]);
    }
    if (interval.getElapsedTime().asMilliseconds() >= updateTime) {
        if (textureIndex == 1) {
            sprite.setTexture(entityTextures[directionIndex][2]);
            textureIndex = 2;
        } else {
            sprite.setTexture(entityTextures[directionIndex][1]);
            textureIndex = 1;
        }
        interval.restart();
    }
}
// void Entity::shooting(const sf::Vector2f& /*targetPosition*/) {
//
// }
