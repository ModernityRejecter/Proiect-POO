#pragma once

#include "../headers/gameState.h"
#include "../headers/stateID.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../headers/game.h"

class GameOverState : public GameState {
public:
    explicit GameOverState(StateMachine& machine);

    void handleEvent(sf::Event* event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    StateID getID() const override;

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Font font;
    sf::Text title;
    sf::Text score;
    sf::Text instruction;

    sf::Clock clock;
};
