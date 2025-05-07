#include "../headers/projectile.h"
    Projectile::Projectile(const sf::Texture& texture, float startX, float startY, float targetX, float targetY, float speed, float spreadAngle)
        : sprite(texture), speed(speed), lifetime(0.f)
    {
        sf::Vector2u size = texture.getSize();
        sprite.setOrigin(sf::Vector2f(static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f));
        sprite.setPosition(sf::Vector2f(startX, startY));

        sf::Vector2f vec(targetX - startX, targetY - startY);
        float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        direction = (length != 0) ? vec / length : sf::Vector2f(1.f, 0.f);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> angleDist(-spreadAngle / 2, spreadAngle / 2);
        float angleOffset = angleDist(gen); // Unghi de variație

        float radians = angleOffset * (3.14159265f / 180.f);

        float newX = direction.x * std::cos(radians) - direction.y * std::sin(radians);
        float newY = direction.x * std::sin(radians) + direction.y * std::cos(radians);
        direction = sf::Vector2f(newX, newY);
    }

    Projectile::Projectile (const Projectile& other)
        : sprite(other.sprite), direction(other.direction), speed(other.speed), lifetime(other.lifetime) {
        std::cout<<"Proiectilul a fost copiat cu succes";
    }

    Projectile::~Projectile() {
        std::cout<<"Proiectiilul a fost distrus cu succes";
    }

    Projectile& Projectile::operator=(const Projectile& other) {
        if (this != &other) {
            sprite = other.sprite;
            direction = other.direction;
            speed = other.speed;
            lifetime = other.lifetime;
            // std::cout<<"Copiere efectuata cu succes"<<std::endl; cauzeaza lag
        }
        return *this;
    }

    std::ostream& operator<<(std::ostream& info, const Projectile& projectile) {
        info <<"Projectile speed : "<< projectile.speed<<std::endl
             <<"Projectile elapsed lifetime : "<<projectile.lifetime<<std::endl
             <<"Projectile position : ("<<projectile.sprite.getPosition().x <<", "<<projectile.sprite.getPosition().y<<")";
        return info;
    }

    void Projectile::update(float deltaTime) {
        sprite.move(direction * speed * deltaTime);
        lifetime += deltaTime;
    }

    bool Projectile::isAlive() const {
        return lifetime < maxLifetime;
    }
    void Projectile::draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

