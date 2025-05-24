#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <algorithm>
#include <random>

#include "../headers/player.h"
#include "../headers/imp.h"
#include "../headers/entity.h"
#include "../headers/projectile.h"
#include "../headers/enemySpawner.h"
#include "../headers/hud.h"
#include "../headers/stateMachine.h"
#include "../headers/pauseState.h"
#include "../headers/cyberDemon.h"
class Game {
private:
    sf::RenderWindow window;
    bool shouldExit;

    sf::Music soundTrack;
    std::vector<std::string> trackPaths;
    float musicVolume;
    int currentMusicIndex;

    sf::Texture background;
    sf::Sprite backgroundSprite;
    sf::Texture hudTexture;
    sf::Sprite hudSprite;

    sf::View view;

    Hud ammo;
    Hud health;
    Hud armor;

    StateMachine states;
    std::vector<std::shared_ptr<Entity>> entities;
    EnemySpawner enemySpawner;

    void processEvents();
    void update(float deltaTime);
    void render();
    void musicHandler(const sf::Event* event);
    void loadTracks();
    void onResize(float width, float height);
    sf::Vector2f getGlobalBounds() const;

public:
    Game();
    void run();
    friend std::ostream& operator<<(std::ostream& info, const Game &game);
};
