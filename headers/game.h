#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include "player.h"
#include "hud.h"
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
    Hud hud;

public:
    Game();
    void loadTracks();
    void run();
    friend std::ostream& operator<<(std::ostream& info, const Game &game);

private:
    void processEvents();
    // bool isWithinBounds();
    void update(float deltaTime);
    void render();
    void musicHandler();
};