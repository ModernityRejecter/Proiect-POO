#pragma once
#include "../headers/gameState.h"
#include "../headers/stateID.h"

class PauseState : public GameState {
public:
    explicit PauseState(StateMachine& machine);
    void handleEvent(sf::Event* event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    StateID getID() const override;

private:
    sf::Font font;
    sf::Text pauseText;
    sf::Font instructionFont;
    sf::Text instruction;
    sf::Texture pauseTexture;
    sf::Sprite pauseSprite;
    sf::Clock clock;
};