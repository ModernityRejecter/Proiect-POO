#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include "player.h"
#include "../headers/hud.h"
#include <random>

class Game {
private:
    sf::RenderWindow window;
    Player player;
    bool shouldExit = false;
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

    void processEvents();
    // bool isWithinBounds();
    void update(float deltaTime);
    void render();
    void musicHandler();
    void loadTracks();
    void onResize(float width, float height);
    sf::Vector2f getGlobalBounds() const;

public:
    Game();
    void run();
    friend std::ostream& operator<<(std::ostream& info, const Game &game);
};