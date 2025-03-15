#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

class Player {
private:
    sf::Texture texture; // Trebuie declarat ÎNAINTE de sprite
    sf::Sprite sprite;
    float speed;

public:
    Player(const std::string& texturePath, float x, float y, float speed)
        : texture(texturePath), sprite(texture), speed(speed) {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Eroare la încărcarea imaginii jucătorului!\n";
            exit(EXIT_FAILURE);
        }
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(x, y));
    }

    void move(float offsetX, float offsetY) {
        sprite.move({offsetX, offsetY});
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

        move(movement.x, movement.y);
    }

    void draw (sf::RenderWindow& window) const {
        window.draw(sprite);
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
          player("../assets/plr_sprite1.png", 400.f, 300.f, 200.f) {
        window.setVerticalSyncEnabled(true);
    }

    void run() {
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
        }
        if (shouldExit) {
            window.close();
            std::cout << "Fereastra a fost închisă.\n";
        }
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