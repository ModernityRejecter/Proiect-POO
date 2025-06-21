#include "../headers/imp.h"

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Imp::entityTextures;
int Imp::updateTime = 150;

Imp::Imp(const sf::Vector2f& position,
         const std::shared_ptr<Player>& targetPlayer,
         const std::string& soundPath,
         const std::string& attackSoundPath)
    : Enemy("./assets/textures/enemies/imp/sprite_1_1.png",
            80,
            120.0f,
            position,
            1.f,
            800.f,
            10,
            1000.f,
            0,
            0,
            targetPlayer,
            soundPath,
            attackSoundPath)
{
    std::string path = "./assets/textures/projectiles/imp_proj.png";
    if (!projectileTexture.loadFromFile(path)) {
        throw TextureLoadException(path);
    }
    sprite.setPosition(position);
    attackSound.setVolume(30);
}

void Imp::loadEntityTextures() {
    std::string basePath = "./assets/textures/enemies/imp/";
    std::string prefix = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture tex;
            if (!tex.loadFromFile(basePath + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png"))
                throw TextureLoadException(basePath + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = tex;
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
std::unique_ptr<Entity> Imp::clone() const {
    return std::make_unique<Imp>(*this);
}

int Imp::onDeath() {
    return 20;
}
