#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../headers/gameState.h"
#include "../headers/stateID.h"
#include <map>
class Game;
class GameState;

class StateMachine {
public:
    void change(StateID id);
    void update(float deltaTime) const;
    void draw(sf::RenderWindow& window) const;
    void handleEvent(sf::Event& ev) const;
    GameState* getCurrentState() const;

    StateMachine(const StateMachine &other);

    StateMachine(StateMachine &&other) noexcept;

    StateMachine & operator=(const StateMachine &other);

    friend void swap(StateMachine &lhs, StateMachine &rhs) noexcept;

    StateMachine & operator=(StateMachine &&other) noexcept;

private:
    std::map<StateID, std::unique_ptr<GameState>> states;
    GameState* currentState = nullptr;
};
