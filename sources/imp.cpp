#include "../headers/imp.h"
#include "../headers/player.h"
#include <cmath>
#include <algorithm>

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Imp::entityTextures;

Imp::Imp(sf::Vector2f position, Player* playerPtr)
    : Entity("./assets/textures/enemies/imp/sprite_3_1.png", 100, 100, 100.f, position),
      targetPlayer(playerPtr)
{
    if (!projectileTexture.loadFromFile("./assets/textures/projectiles/imp_proj.png")) {
        std::cerr << "Eroare la incarcare textura proiectil imp!\n";
        exit(EXIT_FAILURE);
    }
    sprite.setPosition(position);
}

void Imp::loadEntityTextures() {
    std::string path = "./assets/textures/";
    std::string type = "enemies/imp/";
    std::string test = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture entityTexture;
            std::ignore = entityTexture.loadFromFile(path + type + test + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = entityTexture;
        }
    }
}

void Imp::tryToShoot() {
    if (!targetPlayer) return;

    sf::Vector2f impPos = sprite.getPosition();
    sf::Vector2f playerPos = targetPlayer->getPosition();

    float dx = playerPos.x - impPos.x;
    float dy = playerPos.y - impPos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist <= shootRange) {
        if (shootClock.getElapsedTime().asSeconds() >= shootInterval) {
            float startX = impPos.x + sprite.getGlobalBounds().size.x  / 2.f;
            float startY = impPos.y + sprite.getGlobalBounds().size.y / 2.f;

            float targetX = playerPos.x + targetPlayer->getSize().x / 2.f;
            float targetY = playerPos.y + targetPlayer->getSize().y / 2.f;

            float projSpeed = 500.f;
            float spreadAngle = 5.f;

            Projectile newProj(projectileTexture, startX, startY, targetX, targetY, projSpeed, spreadAngle);
            impProjectiles.push_back(std::move(newProj));
            shootClock.restart();
        }
    }
}

void Imp::move(float deltaTime) {
    sf::Vector2f zombiePos = sprite.getPosition();
    sf::Vector2f playerPos = targetPlayer->getPosition();

    sf::Vector2f diff = playerPos - zombiePos;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist > 0.0f) {
        sf::Vector2f dir = diff / dist;
        sprite.move(dir * speed * deltaTime);
        position = sprite.getPosition();
    }
}

void Imp::update(float deltaTime) {
    if (!targetPlayer) return;
    move(deltaTime);

    aimPosition = targetPlayer->getPosition();
    findDirection();
    idleAnimation();
    tryToShoot();

    for (auto& p : impProjectiles) {
        p.update(deltaTime);
    }
    std::erase_if(impProjectiles, [](const Projectile& p) {
        return !p.isAlive();
    });
}

void Imp::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    for (const auto& p : impProjectiles) {
        p.draw(window);
    }
}

void Imp::idleAnimation() {
    if (previousDirectionIndex != directionIndex) {
        sprite.setTexture(entityTextures[directionIndex][1]);
    }
    if (interval.getElapsedTime().asMilliseconds() >= 300) {
        if (textureIndex == 1) {
            sprite.setTexture(entityTextures[directionIndex][2]);
            textureIndex = 2;
        }
        else {
            sprite.setTexture(entityTextures[directionIndex][1]);
            textureIndex = 1;
        }
        interval.restart();
    }
}
