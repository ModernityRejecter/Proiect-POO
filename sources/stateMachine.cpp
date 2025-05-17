#include "../headers/stateMachine.h"
#include "../headers/mainMenuState.h"
#include "../headers/playState.h"
#include "../headers/pauseState.h"

StateMachine::StateMachine()
    : current(nullptr) {}

void StateMachine::change(StateID id) {
    switch (id) {
        case StateID::MainMenu:
            current = std::make_unique<MainMenuState>(*this);
        break;
        case StateID::Play:
            current = std::make_unique<PlayState>(*this);
        break;
        case StateID::Pause:
            current = std::make_unique<PauseState>(*this);
        break;
    }
}

void StateMachine::handleEvent(sf::Event& ev) const {
    if (current) current->handleEvent(&ev);
}

void StateMachine::update(float deltaTime) const {
    if (current) current->update(deltaTime);
}

void StateMachine::draw(sf::RenderWindow& window) const{
    if (current) current->draw(window);
}

GameState* StateMachine::getCurrentState() const{
    return current.get();
}

