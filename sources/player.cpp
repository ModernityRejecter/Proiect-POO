#include "../headers/player.h"

    Player::Player(const std::string& texturePath, float x, float y, float speed)
        :texture(texturePath), sprite(texture), speed(speed), textureIndex(1), directionIndex(1), previousDirectionIndex(1)
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

    void Player::loadPlayerTextures() {
        sf::Texture playerTexture("./assets/textures/player/idle/plr_sprite_d1.png");
        playerTextures[1][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_d2.png");
        playerTextures[1][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sd1.png");
        playerTextures[2][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sd2.png");
        playerTextures[2][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_s1.png");
        playerTextures[3][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_s2.png");
        playerTextures[3][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sa1.png");
        playerTextures[4][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sa2.png");
        playerTextures[4][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_a1.png");
        playerTextures[5][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_a2.png");
        playerTextures[5][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wa1.png");
        playerTextures[6][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wa2.png");
        playerTextures[6][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_w1.png");
        playerTextures[7][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_w2.png");
        playerTextures[7][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wd1.png");
        playerTextures[8][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wd2.png");
        playerTextures[8][2] = playerTexture;

        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_d1.png");
        playerTextures[9][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_d2.png");
        playerTextures[9][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_sd1.png");
        playerTextures[10][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_sd2.png");
        playerTextures[10][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_s1.png");
        playerTextures[11][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_s2.png");
        playerTextures[11][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_sa1.png");
        playerTextures[12][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_sa2.png");
        playerTextures[12][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_a1.png");
        playerTextures[13][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_a2.png");
        playerTextures[13][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_wa1.png");
        playerTextures[14][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_wa2.png");
        playerTextures[14][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_w1.png");
        playerTextures[15][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_w2.png");
        playerTextures[15][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_wd1.png");
        playerTextures[16][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_wd2.png");
        playerTextures[16][2] = playerTexture;

    }

    void Player::move(float offsetX, float offsetY) {
        sprite.move({offsetX, offsetY});
    }
    void Player::shootingAnimation() {
        if(static_cast<float>(interval.getElapsedTime().asMilliseconds()) >= 1000.0f / weapons[currentWeaponIndex].getFireRate()) {
            if (textureIndex == 1) {
                sprite.setTexture(playerTextures[directionIndex + 8][textureIndex]);
                textureIndex = 2;
            }
            else {
                sprite.setTexture(playerTextures[directionIndex + 8][textureIndex]);
                textureIndex = 1;
            }
            interval.restart();
        }
    }
    void Player::loadWeaponsAttributes() {
        weapons.clear();
        weapons.emplace_back("Plasma Rifle", "./assets/textures/projectiles/plasma_proj1.png", 800.0f, 20.0f, 200, 200, 0.0f);
        weapons.emplace_back("BFG", "./assets/textures/projectiles/bfg_proj1.png", 1000.0f, 0.3f, 4, 4, 0.0f);
        weapons.emplace_back("Rocket Launcher", "./assets/textures/projectiles/rocket_proj1.png", 600.0f, 0.7f, 16, 16, 0.0f);
        weapons.emplace_back("Chaingun", "./assets/textures/projectiles/chaingun_proj2.png", 2000.0f, 50.0f, 999, 999, 10.0f);
    }
    void Player::idleAnimation() {
        if (previousDirectionIndex != directionIndex) {
            sprite.setTexture(playerTextures[directionIndex][1]);
        }
        if (interval.getElapsedTime().asMilliseconds() >= 300) {
            if (textureIndex == 1) {
                sprite.setTexture(playerTextures[directionIndex][2]);
                textureIndex = 2;
                // std::cout <<"am actualizat ";
            }
            else {
                sprite.setTexture(playerTextures[directionIndex][1]);
                textureIndex = 1;
                // std::cout <<"am actualizat ";
            }
            // std::cout << textureIndex << std::endl;
            interval.restart();
        }
        // weapons[currentWeaponIndex].passiveReload();
    }

    void Player::update(float deltaTime, unsigned int width, unsigned int height) {

        sf::Vector2f movement(0.f, 0.f);
        sf::Vector2f position = getPosition();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += speed * deltaTime;

        sf::Vector2i mousePos = sf::Mouse::getPosition();
        float angle = std::atan2(static_cast<float>(mousePos.y) - position.y, static_cast<float>(mousePos.x) - position.x) * 180.f / 3.14f;

        previousDirectionIndex = directionIndex;
        if (angle >= -22.5 && angle < 22.5)
            directionIndex = 1;
        else if (angle >= 22.5 && angle < 67.5)
            directionIndex = 2;
        else if (angle >= 67.5 && angle < 112.5)
            directionIndex = 3;
        else if (angle >= 112.5 && angle < 157.5)
            directionIndex = 4;
        else if (angle >= 157.5 || angle < -157.5)
            directionIndex = 5;
        else if (angle >= -157.5 && angle < -112.5)
            directionIndex = 6;
        else if (angle >= -112.5 && angle < -67.5)
            directionIndex = 7;
        else if (angle >= -67.5 && angle < -22.5)
            directionIndex = 8;

        // float halfWidth = static_cast<float>(getSize().x) / 2.f;
        // float halfHeight = static_cast<float>(getSize().y) / 2.f;

        float minX = 0;
        float maxX = static_cast<float>(width) - static_cast<float>(getSize().x);
        float minY = 0;
        float maxY = static_cast<float>(height) - static_cast<float>(getSize().y) - 192.0f * static_cast<float>(height)/1080;
        sf::Vector2f newPos = position + movement;

        newPos.x = std::max(minX, std::min(newPos.x, maxX));
        newPos.y = std::max(minY, std::min(newPos.y, maxY));
        sprite.setPosition(newPos);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            Weapon& currentWeapon = weapons[currentWeaponIndex];
            if (currentWeapon.canShoot()) {
                shootingAnimation();
                currentWeapon.resetFireClock();

                sf::Vector2f playerCenter = {
                    position.x + static_cast<float>(texture.getSize().x) / 2.0f,
                    position.y + static_cast<float>(texture.getSize().y) / 2.0f
                };

                playerProjectiles.push_back(currentWeapon.createProjectile(
                    playerCenter.x,
                    playerCenter.y,
                    static_cast<float>(mousePos.x),
                    static_cast<float>(mousePos.y)
                ));
            }
        }
        else {
            idleAnimation();
        }

        for (auto& projectile : playerProjectiles) {
            projectile.update(deltaTime);
        }

        std::erase_if(playerProjectiles, [](const Projectile& p) { return !p.isAlive(); });
        weaponsHandler();
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

    sf::Vector2f Player::getPosition() const {
        return sprite.getPosition();
    }
    sf::Vector2u Player::getSize() const {
        return texture.getSize();
    }
    // size_t getWeaponIndex() const {
    //     return currentWeaponIndex;
    // }
    int Player::getWeaponAmmoCount() const {
        return weapons[currentWeaponIndex].getAmmoCount();
    }
    // int Player::getWeaponMaxAmmo() const {
    //     return weapons[currentWeaponIndex].getMaxAmmo();
    // }
