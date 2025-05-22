#include "../headers/projectile.h"
#include "../headers/entity.h"

Projectile::Projectile(const sf::Texture& texture,
                       float startX,
                       float startY,
                       float targetX,
                       float targetY,
                       float speed,
                       float spreadAngle,
                       int damage,
                       std::weak_ptr<Entity> ownerPtr)
    : sprite(texture),
      direction(0.f, 0.f),
      speed(speed),
      lifetime(0.f),
      damage(damage),
      owner(std::move(ownerPtr)),
      active(true)
{
    sf::Vector2u sz = texture.getSize();
    sprite.setOrigin(sf::Vector2f(static_cast<float>(sz.x)/2.0f,
                                  static_cast<float>(sz.y)/2.0f));
    sprite.setPosition({ startX, startY });

    sf::Vector2f vec{ targetX - startX, targetY - startY };
    float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    direction = (length != 0.f) ? (vec / length) : sf::Vector2f(1.f, 0.f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(-spreadAngle/2.0f, spreadAngle/2.0f);
    float angleOffset = angleDist(gen) * (3.14159265f/180.f);

    float newX = direction.x * std::cos(angleOffset) - direction.y * std::sin(angleOffset);
    float newY = direction.x * std::sin(angleOffset) + direction.y * std::cos(angleOffset);
    direction = { newX, newY };
}

Projectile::Projectile(const Projectile& other)
    : sprite(other.sprite),
      direction(other.direction),
      speed(other.speed),
      lifetime(other.lifetime),
      damage(other.damage),
      owner(other.owner),
      active(other.active) {

}

Projectile& Projectile::operator=(const Projectile& other) {
    if (this != &other) {
        sprite = other.sprite;
        direction = other.direction;
        speed = other.speed;
        lifetime = other.lifetime;
        damage = other.damage;
        owner = other.owner;
        active = other.active;
    }
    return *this;
}

Projectile::~Projectile() = default;

void Projectile::update(float deltaTime) {
    if (!active) return;
    sprite.move(direction * speed * deltaTime);
    lifetime += deltaTime;
    if (lifetime >= maxLifetime) {
        active = false;
    }
}

bool Projectile::isAlive() const {
    return active;
}

void Projectile::deactivate() {
    active = false;
}

int Projectile::getDamage() const {
    return damage;
}

sf::FloatRect Projectile::getBounds() const {
    return sprite.getGlobalBounds();
}

std::shared_ptr<Entity> Projectile::getOwner() const {
    return owner.lock();
}

void Projectile::draw(sf::RenderWindow& window) const {
    if (active) {
        window.draw(sprite);
    }
}
