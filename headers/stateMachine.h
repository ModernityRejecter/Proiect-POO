#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../headers/gameState.h"
#include "../headers/stateID.h"

class Game;
class GameState;

class StateMachine {
public:
    void push(StateID id);
    void pop();
    explicit StateMachine() = default;
    void change(StateID id);
    void handleEvent(sf::Event& ev) const;
    void update(float deltaTime) const;
    void draw(sf::RenderWindow& window) const;
    [[nodiscard]] GameState* getCurrentState() const;

private:

    std::vector<std::unique_ptr<GameState>> states;
};