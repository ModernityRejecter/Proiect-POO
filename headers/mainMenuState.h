#pragma once

#include "../headers/gameState.h"
#include "../headers/stateID.h"
#include "../headers/stateMachine.h"

class MainMenuState : public GameState {
public:
    explicit MainMenuState(StateMachine& machine);
    void handleEvent(sf::Event* event) override;
    [[maybe_unused]] void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    StateID getID() const override;

private:
    sf::Font fontLeft;
    sf::Font fontRight;
    sf::Text titleLeft;
    sf::Text titleLeftExtra;
    sf::Text titleRight;
    sf::Text titleRightExtra;
    sf::Texture menuTexture;
    sf::Sprite menuSprite;
    sf::Text instruction;
    sf::Clock clock;
};