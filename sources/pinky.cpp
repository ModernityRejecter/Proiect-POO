#include "../headers/pinky.h"

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Pinky::entityTextures;
int Pinky::updateTime = 75;

Pinky::Pinky(const sf::Vector2f& position,
         const std::shared_ptr<Player>& targetPlayer)
    : Enemy("./assets/textures/enemies/pinky/sprite_1_1.png",
            140,
            250.f,
            position,
            2.f,
            50.f,
            50.f,
            1000.f,
            0,
            0,
            targetPlayer)
{
    if (!projectileTexture.loadFromFile("./assets/textures/projectiles/pinky_proj.png")) {
        std::cerr << "Eroare la incarcare textura proiectil pinky!\n";
        exit(EXIT_FAILURE);
    }
    sprite.setPosition(position);
}

void Pinky::loadEntityTextures() {
    std::string basePath = "./assets/textures/enemies/pinky/";
    std::string prefix = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture texture = sf::Texture(basePath + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = texture;
        }
    }
}

void Pinky::update(float deltaTime) {
    auto playerShared = targetPlayer.lock();
    if (!playerShared) return;

    move(deltaTime);

    sf::Vector2f playerPos = playerShared->getPosition();
    aimPosition = playerPos;
    findDirection();
    idleAnimation(entityTextures, updateTime);
    tryToShoot();

    for (auto& p : projectiles) {
        p->update(deltaTime);
    }
    std::erase_if(
        projectiles,
        [](const std::shared_ptr<Projectile>& p) {
            return !p->isAlive();
        }
    );
}
// cppcheck-suppress unusedFunction
std::unique_ptr<Entity> Pinky::clone() const {
    return std::make_unique<Pinky>(*this);
}

