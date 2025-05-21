#include "../headers/imp.h"

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Imp::entityTextures;

Imp::Imp(sf::Vector2f position, Player* playerPtr) : Entity("./assets/textures/enemies/imp/sprite_3_1.png", 100, 100, 100, position), targetPlayer(playerPtr) {
} ;
void Imp::update(float deltaTime) {
    if (!targetPlayer) return;
    move(deltaTime);
    // functia este un prototip functionant dar incomplet
    aimPosition = targetPlayer->getPosition();
    findDirection();
    idleAnimation();
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

void Imp::loadEntityTextures() {
    std::string path = "./assets/textures/";
    std::string type = "enemies/imp/";
    std::string test = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture entityTexture = sf::Texture(path + type + test + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = entityTexture;
            // entityTexture = sf::Texture(path + type + "shooting/" + test + std::to_string(i + 8) + "_" + std::to_string(j) + ".png");
            // entityTextures[i + 8][j] = entityTexture;
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

