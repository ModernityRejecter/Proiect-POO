#pragma once
#include <SFML/Graphics.hpp>
#include "../headers/stateMachine.h"
#include "../headers/stateID.h"
class StateMachine;

class GameState {
public:
    explicit GameState(StateMachine& machine) : machine(machine) {}
    virtual ~GameState() = default;

    virtual void handleEvent(sf::Event* event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual StateID getID() const = 0;

protected:
    StateMachine& machine;
};