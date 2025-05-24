#include "../headers/cyberDemon.h"

std::unordered_map<int, std::unordered_map<int, sf::Texture>> CyberDemon::entityTextures;
int CyberDemon::updateTime = 200;

CyberDemon::CyberDemon(const sf::Vector2f& position,
         const std::shared_ptr<Player>& targetPlayer)
    : Enemy("./assets/textures/enemies/cyber_demon/sprite_1_1.png",
            400,
            60,
            position,
            2.2f,
            500.f,
            30,
            400.f,
            targetPlayer)
{
    if (!projectileTexture.loadFromFile("./assets/textures/projectiles/cyber_demon_proj.png")) {
        std::cerr << "Eroare la incarcare textura proiectil mancubus!\n";
        exit(EXIT_FAILURE);
    }
    sprite.setPosition(position);
}

void CyberDemon::loadEntityTextures() {
    std::string basePath = "./assets/textures/enemies/cyber_demon/";
    std::string prefix   = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture texture = sf::Texture(basePath + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = texture;
        }
    }
}

void CyberDemon::update(float deltaTime) {
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
std::unique_ptr<Entity> CyberDemon::clone() const {
    return std::make_unique<CyberDemon>(*this);
}

