#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../headers/gameState.h"
#include "../headers/stateID.h"

class Game;
class GameState;

class StateMachine {
public:
    explicit StateMachine(Game& game);

    // switch to a new state
    void change(StateID id);

    // forward calls
    void handleEvent(sf::Event& ev) const;
    void update(float deltaTime) const;
    void draw(sf::RenderWindow& window) const;
    GameState* getCurrentState() const;
private:
    Game& game;
    std::unique_ptr<GameState> current;
};