#include "../headers/entity.h"
#include <cmath>

static constexpr float LOGICAL_WIDTH  = 1920;
static constexpr float LOGICAL_HEIGHT = 1080;

Entity::Entity (const std::string& texturePath, int maxHealth, int health, float speed, sf::Vector2f position)
    : Drawable(texturePath, position),
      maxHealth(maxHealth),
      health(health),
      speed(speed),
      directionIndex(1),
      previousDirectionIndex(1),
      textureIndex(1) {

}

int Entity::findDirection() {
    float angle = std::atan2(aimPosition.y - position.y, aimPosition.x - position.x)* 180.f / 3.14159265f;

    previousDirectionIndex = directionIndex;
    if (angle >= -22.5 && angle < 22.5)
        directionIndex = 1;
    else if (angle >= 22.5 && angle < 67.5)
        directionIndex = 2;
    else if (angle >= 67.5 && angle < 112.5)
        directionIndex = 3;
    else if (angle >= 112.5 && angle < 157.5)
        directionIndex = 4;
    else if (angle >= 157.5 || angle < -157.5)
        directionIndex = 5;
    else if (angle >= -157.5 && angle < -112.5)
        directionIndex = 6;
    else if (angle >= -112.5 && angle < -67.5)
        directionIndex = 7;
    else if (angle >= -67.5 && angle < -22.5)
        directionIndex = 8;

    return directionIndex;
}

void Entity::move(float /*deltaTime*/) {
    sf::Vector2f movement(0.f, 0.f);
    position = getPosition();

    /* ENTITY AI CODE WILL BE HERE */

    // float halfWidth = static_cast<float>(getSize().x) / 2.f;
    // float halfHeight = static_cast<float>(getSize().y) / 2.f;

    float minX = 0;
    float maxX = static_cast<float>(LOGICAL_WIDTH) - static_cast<float>(getSize().x);
    float minY = 0;
    float maxY = static_cast<float>(LOGICAL_HEIGHT) - static_cast<float>(getSize().y) - 192.0f;
    sf::Vector2f newPos = position + movement;

    // newPos.x = std::max(minX, std::min(newPos.x, maxX));
    // newPos.y = std::max(minY, std::min(newPos.y, maxY));
    // sprite.setPosition(newPos);
    if (newPos.x < maxX && newPos.x > minX && newPos.y < maxY && newPos.y > minY)
        sprite.move(movement);
}
Entity::~Entity() = default;
// cppcheck-suppress unusedFunction
void Entity::shooting(const sf::Vector2f& targetPosition) {
    [[maybe_unused]] float angle = std::atan2((targetPosition.y) - position.y, (targetPosition.x) - position.x) * 180.f / 3.14f;

    previousDirectionIndex = directionIndex;
    // if (angle >= -22.5 && angle < 22.5)
    //     directionIndex = 1;
    // else if (angle >= 22.5 && angle < 67.5)
    //     directionIndex = 2;
    // else if (angle >= 67.5 && angle < 112.5)
    //     directionIndex = 3;
    // else if (angle >= 112.5 && angle < 157.5)
    //     directionIndex = 4;
    // else if (angle >= 157.5 || angle < -157.5)
    //     directionIndex = 5;
    // else if (angle >= -157.5 && angle < -112.5)
    //     directionIndex = 6;
    // else if (angle >= -112.5 && angle < -67.5)
    //     directionIndex = 7;
    // else if (angle >= -67.5 && angle < -22.5)
    //     directionIndex = 8;

    // logica de mai sus este momentan comentata deoarece nu este in folos si cauzeaza probleme la bife

    // else {
    //     idleAnimation();
    // }
    // std::erase_if(playerProjectiles, [](const Projectile& p) { return !p.isAlive(); });
}

void Entity::update(float deltaTime) {
    move(deltaTime);
}

int Entity::getHealth() const {
    return health;
}
