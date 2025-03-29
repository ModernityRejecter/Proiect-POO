#include <iostream>
#include <vector>
#include <complex>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map>
#include <cmath>


class Projectile {
private:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float lifetime;
    static constexpr float maxLifetime = 3.0f;

public:
    Projectile(const sf::Texture& texture, float startX, float startY, float targetX, float targetY, float speed)
        : sprite(texture), speed(speed), lifetime(0.f)
    {
        sf::Vector2u size = texture.getSize();
        sprite.setOrigin(sf::Vector2f(static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f));
        sprite.setPosition(sf::Vector2f(startX, startY));

        sf::Vector2f vec(targetX - startX, targetY - startY);
        float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        direction = (length != 0) ? vec / length : sf::Vector2f(1.f, 0.f);
    }

    Projectile (const Projectile& other)
        : sprite(other.sprite), direction(other.direction), speed(other.speed), lifetime(other.lifetime) {
        std::cout<<"Proiectilul a fost copiat cu succes";
    }

    ~Projectile() {
        std::cout<<"Proiectiilul a fost distrus cu succes";
    }

    Projectile& operator=(const Projectile& other) {
        if (this != &other) {
            sprite = other.sprite;
            direction = other.direction;
            speed = other.speed;
            lifetime = other.lifetime;
            // std::cout<<"Copiere efectuata cu succes"<<std::endl; cauzeaza lag
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& info, const Projectile& projectile) {
        info <<"Projectile speed : "<< projectile.speed<<std::endl
             <<"Projectile elapsed lifetime : "<<projectile.lifetime<<std::endl
             <<"Projectile position : ("<<projectile.sprite.getPosition().x <<", "<<projectile.sprite.getPosition().y<<")";
        return info;
    }

    void update(float deltaTime) {
        sprite.move(direction * speed * deltaTime);
        lifetime += deltaTime;
    }

    bool isAlive() const {
        return lifetime < maxLifetime;
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }
};

class Weapon {
private:
    std::string name;
    sf::Texture projectileTexture;
    std::vector<Projectile> projectiles;
    float fireRate;
    float projectileSpeed;
    sf::Clock fireClock;
    bool canShoot = true;

public:
    Weapon(const std::string& name, const std::string& projectilePath, float speed, float rate)
        : name(name), fireRate(rate), projectileSpeed(speed)
    {
        if (!projectileTexture.loadFromFile(projectilePath)) {
            std::cerr << "Eroare la incarcarea texturii pentru " << name << "\n";
            exit(EXIT_FAILURE);
        }
    }

    friend std::ostream& operator<<(std::ostream& info, const Weapon& weapon) {
        info <<"Weapon name : "<<weapon.name<<std::endl
             <<"Firerate : "<<weapon.fireRate<<std::endl;

        for (const auto& projectile : weapon.projectiles) {
            std::cout<<"Projectile information : "<<std::endl<<projectile;
        }
        return info;
    }
    void update(float deltaTime) {

        for (auto& p : projectiles) p.update(deltaTime);

        std::erase_if(projectiles, [](const Projectile& p) { return !p.isAlive(); });

        if (!canShoot && fireClock.getElapsedTime().asSeconds() >= 1.0f/fireRate) {
            canShoot = true;
        }
    }

    void fire(float startX, float startY, float targetX, float targetY) {
        if (canShoot) {
            projectiles.emplace_back(projectileTexture, startX, startY, targetX, targetY, projectileSpeed);
            fireClock.restart();
            canShoot = false;
        }
    }

    void drawProjectiles(sf::RenderWindow& window) const {
        for (const auto& p : projectiles) p.draw(window);
    }

    float getFireRate() const {
        return fireRate;
    }
};

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    sf::Clock interval;
    std::unordered_map<int, std::unordered_map<int, sf::Texture>> playerTextures;
    int textureIndex;
    int directionIndex;
    int previousDirectionIndex;
    std::vector<Weapon> weapons;
    size_t currentWeaponIndex = 0;

public:
    Player(const std::string& texturePath, float x, float y, float speed)
        :texture(texturePath), sprite(texture), speed(speed), textureIndex(1), directionIndex(1), previousDirectionIndex(1)
    {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Eroare la incarcarea imaginii jucatorului!\n";
            exit(EXIT_FAILURE);
        }
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(x, y));

        weapons.emplace_back("Plasma Rifle", "./assets/plasma_proj1.png", 500.0f, 20.0f);
    }
    // afisarea cauzeaza lag in cazul in care sunt active multe proiectile
    // motiv pentru care este apelata la 5 secunde (trebuie gandita putin mai bine)
    friend std::ostream& operator<<(std::ostream& info, const Player& player) {
        info << "Player position : ("<< player.sprite.getPosition().x<<", "<<player.sprite.getPosition().y<<")"<<std::endl
             << "Current direction : "<<player.directionIndex<<std::endl
             << "Current weapon information: "<<player.weapons[player.currentWeaponIndex]<<std::endl;
        return info;
    }
    void loadPlayerTextures() {
        sf::Texture playerTexture("./assets/plr_sprite_d1.png");
        playerTextures[1][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_d2.png");
        playerTextures[1][2] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_sd1.png");
        playerTextures[2][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_sd2.png");
        playerTextures[2][2] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_s1.png");
        playerTextures[3][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_s2.png");
        playerTextures[3][2] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_sa1.png");
        playerTextures[4][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_sa2.png");
        playerTextures[4][2] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_a1.png");
        playerTextures[5][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_a2.png");
        playerTextures[5][2] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_wa1.png");
        playerTextures[6][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_wa2.png");
        playerTextures[6][2] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_w1.png");
        playerTextures[7][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_w2.png");
        playerTextures[7][2] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_wd1.png");
        playerTextures[8][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_sprite_wd2.png");
        playerTextures[8][2] = playerTexture;

        playerTexture = sf::Texture("./assets/plr_shooting_s1.png");
        playerTextures[9][1] = playerTexture;
        playerTexture = sf::Texture("./assets/plr_shooting_s2.png");
        playerTextures[9][2] = playerTexture;

    }

    void move(float offsetX, float offsetY) {
        sprite.move({offsetX, offsetY});
    }
    void shootingAnimation() {
        if(static_cast<float>(interval.getElapsedTime().asMilliseconds()) >= 1000.0f / weapons[currentWeaponIndex].getFireRate()) {
            if (textureIndex == 1) {
                sprite.setTexture(playerTextures[9][textureIndex]);
                textureIndex = 2;
            }
            else {
                sprite.setTexture(playerTextures[9][textureIndex]);
                textureIndex = 1;
            }
            interval.restart();
        }
    }
    // void loadWeaponsAttributes() {
    //     weapons.clear();
    //     weapons.emplace_back("PlasmaRifle", "./assets/plasma_proj1.png", 500.0f, 0.2f);
    // } Work in progress
    void idleAnimation() {
        if (previousDirectionIndex != directionIndex) {
            sprite.setTexture(playerTextures[directionIndex][1]);
        }
        if (interval.getElapsedTime().asMilliseconds() >= 300) {
            if (textureIndex == 1) {
                sprite.setTexture(playerTextures[directionIndex][2]);
                textureIndex = 2;
                std::cout <<"am actualizat ";
            }
            else {
                sprite.setTexture(playerTextures[directionIndex][1]);
                textureIndex = 1;
                std::cout <<"am actualizat ";
            }
            std::cout << textureIndex << std::endl;
            interval.restart();
        }

    }
    void update(float deltaTime) {

        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += speed * deltaTime;


        sf::Vector2f position = getPosition();
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

        move(movement.x, movement.y);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            shootingAnimation();

            sf::Vector2f playerPos = getPosition();
            sf::Vector2u playerSize = getSize();
            weapons[currentWeaponIndex].fire(
                playerPos.x + static_cast<float>(playerSize.x)/2.0f,
                playerPos.y + static_cast<float>(playerSize.y)/2.0f,
                static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y)
            );
        } else {
            idleAnimation();
        }

        weapons[currentWeaponIndex].update(deltaTime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) currentWeaponIndex = 0;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
        weapons[currentWeaponIndex].drawProjectiles(window);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    sf::Vector2u getSize() const {
        return texture.getSize();
    }
};




class Game {
private:
    sf::RenderWindow window;
    Player player;
    bool shouldExit = false;
public:
    Game()
        : window(sf::VideoMode({1920, 1111}), "ETERNAL DOOM", sf::Style::Default),
          player("./assets/plr_sprite_s1.png", 400.f, 300.f, 200.f){
        window.setVerticalSyncEnabled(true);
    }

    void run() {
        player.loadPlayerTextures();
        sf::Clock clock;
        sf::Clock informationClock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            processEvents();
            update(deltaTime);
            render();
            if (informationClock.getElapsedTime().asMilliseconds() >= 5000) {
                std::cout<<player;
                informationClock.restart();
            }
        }
    }
    friend std::ostream& operator<<(std::ostream& info, const Game &game) {
        info <<"Player information : "<< game.player <<std::endl;
        return info;
    }
private:
    void processEvents() {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::Escape) {
                shouldExit = true;
            }
        }
        if (shouldExit) window.close();
    }

    void update(float deltaTime) {
        player.update(deltaTime);
    }

    void render() {
        window.clear();
        player.draw(window);
        window.display();

    }
};

int main() {
    Game game;
    game.run();
    return 0;
}