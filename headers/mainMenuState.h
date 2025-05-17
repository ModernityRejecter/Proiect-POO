#pragma once
#include "../headers/gameState.h"
#include "../headers/stateID.h"

class MainMenuState : public GameState {
public:
    explicit MainMenuState(StateMachine& machine);
    void handleEvent(sf::Event* event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    StateID getID() const override;
private:
    sf::Font font;
    sf::Text title;
};