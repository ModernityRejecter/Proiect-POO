#include <iostream>
#include <vector>
#include <complex>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map>
#include <cmath>

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

public:
    Player(const std::string& texturePath, float x, float y, float speed)
        : texture(texturePath), sprite(texture), speed(speed), textureIndex(1), directionIndex(1), previousDirectionIndex(1) {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Eroare la incarcarea imaginii jucatorului!\n";
            exit(EXIT_FAILURE);
        }
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(x, y));
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
    }

    void move(float offsetX, float offsetY) {
        sprite.move({offsetX, offsetY});
    }
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
        sf::Vector2i mousePosition = sf::Mouse::getPosition();

        float angle = std::atan2(static_cast<float>(mousePosition.y) - position.y, static_cast<float>(mousePosition.x) - position.x) * 180.f / 3.14f;
        std::cout<<angle;
        previousDirectionIndex = directionIndex;

        if (angle >= -22.5 && angle < 22.5) {
            directionIndex = 1;
        } else if (angle >= 22.5 && angle < 67.5) {
            directionIndex = 2;
        } else if (angle >= 67.5 && angle < 112.5) {
            directionIndex = 3;
        } else if (angle >= 112.5 && angle < 157.5) {
            directionIndex = 4;
        } else if (angle >= 157.5 || angle < -157.5) {
            directionIndex = 5;
        } else if (angle >= -157.5 && angle < -112.5) {
            directionIndex = 6;
        } else if (angle >= -112.5 && angle < -67.5) {
            directionIndex = 7;
        } else if (angle >= -67.5 && angle < -22.5) {
            directionIndex = 8;
        }
        move(movement.x, movement.y);
        idleAnimation();
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    sf::Vector2u getSize() const {
        return texture.getSize();
    }
};

class Projectile {
private:
    static sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float lifetime;
    static constexpr float maxLifetime = 3.0f;

public:
    Projectile(float startX, float startY, float targetX, float targetY, float speed)
        : sprite(texture), speed(speed), lifetime(0.f) {
        if (texture.getSize().x == 0) {
            if (!texture.loadFromFile("./assets/plasma_proj1.png")) {
                std::cerr << "Eroare la incarcarea texturii proiectilului!\n";
                exit(EXIT_FAILURE);
            }
        }
        sprite.setTexture(texture);
        sf::Vector2u size = texture.getSize();
        sprite.setOrigin(sf::Vector2f(static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f));
        sprite.setPosition(sf::Vector2f(startX, startY));

        sf::Vector2f vec(targetX - startX, targetY - startY);
        float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        if (length != 0)
            direction = vec / length;
        else
            direction = sf::Vector2f(1.f, 0.f);
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

sf::Texture Projectile::texture;

class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::vector<Projectile> projectiles;
    bool shouldExit = false;

public:
    Game()
        : window(sf::VideoMode({1920, 1111}), "ETERNAL DOOM", sf::Style::Default),
          player("./assets/plr_sprite_s1.png", 400.f, 300.f, 200.f) {
        window.setVerticalSyncEnabled(true);
    }

    void run() {
        player.loadPlayerTextures();
        sf::Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            processEvents();
            update(deltaTime);
            render();
        }
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
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                    sf::Vector2f playerPos = player.getPosition();
                    sf::Vector2u playerSize = player.getSize();

                    projectiles.emplace_back(
                        playerPos.x + static_cast<float>(playerSize.x) / 2.0f,
                        playerPos.y + static_cast<float>(playerSize.y) / 2.0f,
                        worldPos.x,
                        worldPos.y,
                        500.0f
                    );
                }
            }
        }
        if (shouldExit) window.close();
    }

    void update(float deltaTime) {
        player.update(deltaTime);
        for (auto& projectile : projectiles) projectile.update(deltaTime);
        std::erase_if(projectiles, [](const Projectile& b) { return !b.isAlive(); });
    }

    void render() {
        window.clear();
        player.draw(window);
        for (const auto& projectile : projectiles) projectile.draw(window);
        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}