#include "../headers/player.h"

std::unordered_map<int, std::unordered_map<int, sf::Texture>> Player::entityTextures;

Player::Player(const std::string& texturePath, float x, float y, float speed)
    : Entity(texturePath, 100, 100, speed, 200, 40, {x, y}),
      isShooting(false),
      currentWeaponIndex(0)
{
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Eroare la incarcarea imaginii jucatorului!\n";
        exit(EXIT_FAILURE);
    }
    sprite.setTexture(texture);
    sprite.setPosition({x, y});
    loadWeaponsAttributes();
}

void Player::loadEntityTextures() {
    std::string basePath = "./assets/textures/player/";
    std::string prefix   = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture texture = sf::Texture(basePath + "idle/" + prefix + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = texture;
            texture =  sf::Texture(basePath + "shooting/" + prefix + std::to_string(i + 8) + "_" + std::to_string(j) + ".png");
            entityTextures[i + 8][j] = texture;
        }
    }
}

void Player::loadWeaponsAttributes() {
    weapons.clear();
    weapons.reserve(4);
    weapons.emplace_back("Plasma Rifle", "./assets/textures/projectiles/plasma_proj1.png", 900.0f, 20.0f, 200, 200, 0.0f, "./assets/sounds/plasma.wav", 20);
    weapons.emplace_back("Rocket Launcher", "./assets/textures/projectiles/rocket_proj2.png", 600.0f, 1.2f, 8, 8, 0.0f, "./assets/sounds/rocket_launcher.wav", 200);
    weapons.emplace_back("Chaingun", "./assets/textures/projectiles/chaingun_proj2.png", 2000.0f, 50.0f, 999, 999, 10.0f, "./assets/sounds/pistol.wav", 7);
    weapons.emplace_back("BFG", "./assets/textures/projectiles/bfg_proj1.png", 1200.0f, 0.7f, 5, 5, 0.0f, "./assets/sounds/BFG_9000.wav", 2000);
}

void Player::move(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);
    position = getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * deltaTime;

    float minX = 0.f;
    float maxX = LOGICAL_WIDTH - static_cast<float>(getSize().x);
    float minY = 0.f;
    float maxY = LOGICAL_HEIGHT - 192.0f - static_cast<float>(getSize().y);
    sf::Vector2f newPos = position + movement;

    if (newPos.x < maxX && newPos.x > minX && newPos.y < maxY && newPos.y > minY)
        sprite.move(movement);
}

void Player::playerShooting() {
    findDirection();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        Weapon& currentWeapon = weapons[currentWeaponIndex];
        if (currentWeapon.canShoot()) {
            currentWeapon.playSound();
            shootingAnimation();
            currentWeapon.resetFireClock();

            sf::Vector2f playerCenter = {
                sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.0f,
                sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.0f
            };

            Projectile p = currentWeapon.createProjectile(
                playerCenter.x, playerCenter.y,
                aimPosition.x, aimPosition.y,
                shared_from_this()
            );
            projectiles.push_back(std::make_shared<Projectile>(p));
        }
    }
    else if (!isShooting) {
        idleAnimation(entityTextures, updateTime);
    }

    std::erase_if(
        projectiles,
        [](const std::shared_ptr<Projectile>& p) {
            return !p->isAlive();
        }
    );
}

void Player::weaponsHandler() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) currentWeaponIndex = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) currentWeaponIndex = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) currentWeaponIndex = 2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) currentWeaponIndex = 3;
}

void Player::updateShootingAnimation() {
    if (!isShooting) return;
    float fireRateMs = std::min(1000.0f / weapons[currentWeaponIndex].getFireRate(), 500.0f);
    float elapsedTotal = shotClock.getElapsedTime().asMilliseconds();

    if (elapsedTotal >= fireRateMs / 2 && textureIndex == 2) {
        textureIndex = 1;
        sprite.setTexture(entityTextures[directionIndex + 8][textureIndex]);
    }
    else if (elapsedTotal >= fireRateMs) {
        sprite.setTexture(entityTextures[directionIndex][1]);
        isShooting = false;
        textureIndex = 2;
    }
}

void Player::update(float deltaTime) {
    move(deltaTime);
    playerShooting();
    updateShootingAnimation();
    weaponsHandler();

    for (auto& p : projectiles) {
        p->update(deltaTime);
    }
}

void Player::shootingAnimation() {
    isShooting = true;
    shotClock.restart();
    textureIndex = 2;
    sprite.setTexture(entityTextures[directionIndex + 8][textureIndex]);
}

std::vector<std::shared_ptr<Projectile>>& Player::getProjectiles() {
    return projectiles;
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    for (const auto& p : projectiles) {
        p->draw(window);
    }
}

int Player::getWeaponAmmoCount() const {
    return weapons[currentWeaponIndex].getAmmoCount();
}

int Player::getWeaponMaxAmmo() const {
    return weapons[currentWeaponIndex].getMaxAmmo();
}

int Player::getPlayerArmor() const {
    return armor;
}

void Player::setAim(const sf::Vector2f& worldMousePos) {
    aimPosition = worldMousePos;
}

void Player::addAmmo() {
    for (auto& weapon : weapons ) {
        weapon.addAmmo();
    }
}
std::ostream& operator<<(std::ostream& info, const Player& player) {
    info << "Player position : ("
         << player.sprite.getPosition().x << ", "
         << player.sprite.getPosition().y << ")\n"
         << "Current direction : " << player.directionIndex << "\n"
         << "Current weapon: " << player.weapons[player.currentWeaponIndex] << "\n";
    return info;
}
