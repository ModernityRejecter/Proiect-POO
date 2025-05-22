#include "../headers/imp.h"
#include <cmath>
#include <algorithm>

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Imp::entityTextures;

Imp::Imp(sf::Vector2f position, const std::shared_ptr<Player> &playerPtr)
    : Entity("./assets/textures/enemies/imp/sprite_3_1.png", 100, 100, 100.f, position),
      targetPlayer(playerPtr),
      shootInterval(1.5f),
      shootRange(400.f)
{
    if (!projectileTexture.loadFromFile("./assets/textures/projectiles/imp_proj.png")) {
        std::cerr << "Eroare la incarcare textura proiectil imp!\n";
        exit(EXIT_FAILURE);
    }
    sprite.setPosition(position);
}

void Imp::loadEntityTextures() {
    std::string basePath = "./assets/textures/enemies/imp/";
    std::string prefix   = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture texture = sf::Texture(basePath + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = texture;
        }
    }
}

void Imp::tryToShoot() {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    sf::Vector2f impPos = sprite.getPosition();
    sf::Vector2f playerPos = playerShared->getPosition();

    float dx = playerPos.x - impPos.x;
    float dy = playerPos.y - impPos.y;
    float dist = std::sqrt(dx*dx + dy*dy);

    if (dist <= shootRange && shootClock.getElapsedTime().asSeconds() >= shootInterval) {
        sf::Vector2f impCenter = {
            impPos.x + sprite.getGlobalBounds().size.x  / 2.f,
            impPos.y + sprite.getGlobalBounds().size.y / 2.f
        };
        sf::Vector2f playerCenter = {
            playerPos.x + playerShared->getBounds().size.x  / 2.f,
            playerPos.y + playerShared->getBounds().size.y / 2.f
        };

        auto p = std::make_unique<Projectile>(
            projectileTexture,
            impCenter.x, impCenter.y,
            playerCenter.x, playerCenter.y,
            500.f,
            5.f,
            10,
            shared_from_this()
        );
        projectiles.push_back(std::move(p));
        shootClock.restart();
    }
}

void Imp::move(float deltaTime) {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    sf::Vector2f zombiePos = sprite.getPosition();
    sf::Vector2f playerPos = playerShared->getPosition();

    sf::Vector2f diff = playerPos - zombiePos;
    float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y);

    if (dist > 0.0f) {
        sf::Vector2f dir = diff / dist;
        sprite.move(dir * speed * deltaTime);
        position = sprite.getPosition();
    }
}

void Imp::update(float deltaTime) {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    move(deltaTime);

    sf::Vector2f playerPos = playerShared->getPosition();
    aimPosition = playerPos;
    findDirection();
    idleAnimation();
    tryToShoot();

    for (auto& p : projectiles) {
        p->update(deltaTime);
    }
    std::erase_if(
        projectiles,
        [](const std::unique_ptr<Projectile>& p) {
            return !p->isAlive();
        }
    );
}

void Imp::idleAnimation() {
    if (previousDirectionIndex != directionIndex) {
        sprite.setTexture(entityTextures[directionIndex][1]);
    }
    if (interval.getElapsedTime().asMilliseconds() >= 300) {
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

std::vector<std::unique_ptr<Projectile>>& Imp::getProjectiles() {
    return projectiles;
}

void Imp::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    for (const auto& p : projectiles) {
        p->draw(window);
    }
}
