#include "../headers/stateMachine.h"
#include "../headers/mainMenuState.h"
#include "../headers/playState.h"
#include "../headers/pauseState.h"

void StateMachine::change(StateID id) {
    if (!states.contains(id)) {
        switch (id) {
            case StateID::MainMenu:
                states[id] = std::make_unique<MainMenuState>(*this); break;
            case StateID::Play:
                states[id] = std::make_unique<PlayState>(*this); break;
            case StateID::Pause:
                states[id] = std::make_unique<PauseState>(*this); break;
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


