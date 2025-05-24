#include "../headers/imp.h"
#include <cmath>
#include <algorithm>

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Imp::entityTextures;

Imp::Imp(const sf::Vector2f& position,
         const std::shared_ptr<Player>& targetPlayer)
    : Enemy("./assets/textures/enemies/imp/sprite_1_1.png",
            50,
            120.0f,
            position, targetPlayer)
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

void Imp::update(float deltaTime) {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    move(deltaTime);

    sf::Vector2f playerPos = playerShared->getPosition();
    aimPosition = playerPos;
    findDirection();
    idleAnimation(entityTextures);
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

std::unique_ptr<Entity> Imp::clone() const {
    return std::make_unique<Imp>(*this);
}
