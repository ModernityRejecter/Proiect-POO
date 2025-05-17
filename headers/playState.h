#pragma once
#include "../headers/gameState.h"
#include "../headers/stateID.h"

class PlayState : public GameState {
public:
    explicit PlayState(StateMachine& machine);
    void handleEvent(sf::Event* event) override;
    [[maybe_unused]] void update(float deltaTime) override;
    [[maybe_unused]] void draw(sf::RenderWindow& window) override;
    StateID getID() const override;
};
