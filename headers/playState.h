#pragma once
#include "../headers/gameState.h"
#include "../headers/stateID.h"

class PlayState : public GameState {
public:
    explicit PlayState(StateMachine& machine);
    void handleEvent(sf::Event* event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    StateID getID() const override;
private:
    // your game objects
};
