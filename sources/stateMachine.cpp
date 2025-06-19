#include "../headers/stateMachine.h"
#include "../headers/mainMenuState.h"
#include "../headers/playState.h"
#include "../headers/pauseState.h"
#include "../headers/gameOverState.h"

void StateMachine::change(StateID id) {
    if (!states.contains(id)) {
        switch (id) {
            case StateID::MainMenu:
                states[id] = std::make_unique<MainMenuState>(*this);
            break;
            case StateID::Play:
                states[id] = std::make_unique<PlayState>(*this);
            break;
            case StateID::Pause:
                states[id] = std::make_unique<PauseState>(*this);
            break;
            case StateID::GameOver:
                states[id] = std::make_unique<GameOverState>(*this);
            break;
        }
    }
    currentState = states[id].get();
}

void StateMachine::update(float deltaTime) const {
    if (currentState) currentState->update(deltaTime);
}

void StateMachine::draw(sf::RenderWindow& window) const {
    if (currentState) currentState->draw(window);
}

void StateMachine::handleEvent(sf::Event& event) const {
    if (currentState) currentState->handleEvent(&event);
}

GameState* StateMachine::getCurrentState() const {
    return currentState;
}

StateMachine::StateMachine() {
}


StateMachine::StateMachine(StateMachine &&other) noexcept: states(std::move(other.states)),
                                                           currentState(other.currentState) {
}

StateMachine & StateMachine::operator=(StateMachine &&other) noexcept {
    if (this == &other)
        return *this;
    states = std::move(other.states);
    currentState = other.currentState;
    return *this;
}

void swap(StateMachine &lhs, StateMachine &rhs) noexcept {
    using std::swap;
    swap(lhs.states, rhs.states);
    swap(lhs.currentState, rhs.currentState);
}
