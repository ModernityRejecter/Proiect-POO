#include "../headers/player.h"

static constexpr float LOGICAL_WIDTH  = 1920;
static constexpr float LOGICAL_HEIGHT = 1080;
std::unordered_map<int, std::unordered_map<int, sf::Texture>> Player::entityTextures;

Player::Player(const std::string& texturePath, float x, float y, float speed)
    :Entity(texturePath, 100, 100, speed, {x, y})
{
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Eroare la incarcarea imaginii jucatorului!\n";
        exit(EXIT_FAILURE);
    }
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
    loadWeaponsAttributes();
}
// afisarea cauzeaza lag in cazul in care sunt active multe proiectile
// motiv pentru care este apelata la 5 secunde (trebuie gandita putin mai bine)
std::ostream& operator<<(std::ostream& info, const Player& player) {
    info << "Player position : ("<< player.sprite.getPosition().x<<", "<<player.sprite.getPosition().y<<")"<<std::endl
         << "Current direction : "<<player.directionIndex<<std::endl
         << "Current weapon information: "<<player.weapons[player.currentWeaponIndex]<<std::endl;

    return info;
}

void Player::loadEntityTextures() {
    std::string path = "./assets/textures/";
    std::string type = "player/";
    std::string test = "sprite_";
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 2; j++) {
            sf::Texture entityTexture = sf::Texture(path + type + "idle/" + test + std::to_string(i) + "_" + std::to_string(j) + ".png");
            entityTextures[i][j] = entityTexture;
            entityTexture = sf::Texture(path + type + "shooting/" + test + std::to_string(i + 8) + "_" + std::to_string(j) + ".png");
            entityTextures[i + 8][j] = entityTexture;
        }
    }
}

void Player::shootingAnimation() {
    isShooting = true;
    shotClock.restart();
    textureIndex = 2;
    sprite.setTexture(entityTextures[directionIndex + 8][textureIndex]);
}

void Player::loadWeaponsAttributes() {
    weapons.clear();
    weapons.reserve(4);
    weapons.emplace_back("Plasma Rifle", "./assets/textures/projectiles/plasma_proj1.png", 800.0f, 20.0f, 200, 200, 0.0f, "./assets/sounds/plasma.wav");
    weapons.emplace_back("Rocket Launcher", "./assets/textures/projectiles/rocket_proj1.png", 600.0f, 0.7f, 16, 16, 0.0f, "./assets/sounds/rocket_launcher.wav");
    weapons.emplace_back("Chaingun", "./assets/textures/projectiles/chaingun_proj2.png", 2000.0f, 50.0f, 999, 999, 10.0f, "./assets/sounds/pistol.wav");
    weapons.emplace_back("BFG", "./assets/textures/projectiles/bfg_proj1.png", 1000.0f, 0.3f, 4, 4, 0.0f, "./assets/sounds/BFG_9000.wav");
}

void Player::idleAnimation() {
    if (previousDirectionIndex != directionIndex) {
        sprite.setTexture(entityTextures[directionIndex][1]);
    }
    if (interval.getElapsedTime().asMilliseconds() >= 300) {
        if (textureIndex == 1) {
            sprite.setTexture(entityTextures[directionIndex][2]);
            textureIndex = 2;
            // std::cout <<"am actualizat ";
        }
        else {
            sprite.setTexture(entityTextures[directionIndex][1]);
            textureIndex = 1;
            // std::cout <<"am actualizat ";
        }
        // std::cout << textureIndex << std::endl;
        interval.restart();
    }
    // weapons[currentWeaponIndex].passiveReload();
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

    float minX = 0;
    float maxX = static_cast<float>(LOGICAL_WIDTH) - static_cast<float>(getSize().x);
    float minY = 0;
    float maxY = static_cast<float>(LOGICAL_HEIGHT) - 192 -  static_cast<float>(getSize().y);
    sf::Vector2f newPos = position + movement;

    if (newPos.x < maxX && newPos.x > minX && newPos.y < maxY && newPos.y > minY)
        sprite.move(movement);
}

void Player::playerShooting() {

    findDirection();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        Weapon& currentWeapon = weapons[currentWeaponIndex];
        if (currentWeapon.canShoot()) {
            weapons[currentWeaponIndex].playSound();
            shootingAnimation();
            currentWeapon.resetFireClock();

            sf::Vector2f playerCenter = {
                position.x + static_cast<float>(texture.getSize().x) / 2.0f,
                position.y + static_cast<float>(texture.getSize().y) / 2.0f
            };

            playerProjectiles.push_back(currentWeapon.createProjectile(
                playerCenter.x,
                playerCenter.y,
                aimPosition.x,
                aimPosition.y
            ));
        }
    }
    else if (!isShooting){
        idleAnimation();
    }
    std::erase_if(playerProjectiles, [](const Projectile& p) { return !p.isAlive(); });
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
    for (auto& projectile : playerProjectiles) {
        projectile.update(deltaTime);
    }
}

void Player::weaponsHandler() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) currentWeaponIndex = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) currentWeaponIndex = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) currentWeaponIndex = 2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) currentWeaponIndex = 3;
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    for (const auto& projectile : playerProjectiles) {
        projectile.draw(window);
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
