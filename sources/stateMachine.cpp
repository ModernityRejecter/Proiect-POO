#include "../headers/stateMachine.h"
#include "../headers/mainMenuState.h"
#include "../headers/playState.h"
#include "../headers/pauseState.h"

void StateMachine::change(StateID id) {
    states.clear();
    push(id);
}

void StateMachine::push(StateID id) {
    switch (id) {
        case StateID::MainMenu:
            states.emplace_back(std::make_unique<MainMenuState>(*this));
        break;
        case StateID::Play:
            states.emplace_back(std::make_unique<PlayState>(*this));
        break;
        case StateID::Pause:
            states.emplace_back(std::make_unique<PauseState>(*this));
        break;
    }
}
// cppcheck-suppress unusedFunction
void StateMachine::pop() {
    if (!states.empty()) {
        states.pop_back();
    }
}

void StateMachine::handleEvent(sf::Event& ev) const {
    if (!states.empty()) {
        states.back()->handleEvent(&ev);
    }
}

void StateMachine::update(float deltaTime) const {
    if (!states.empty()) {
        if (states.back()->getID() == StateID::Play) {
            states.back()->update(deltaTime);
        }
    }
}

void StateMachine::draw(sf::RenderWindow& window) const {
    for (auto& state : states) {
        state->draw(window);
    }
}

GameState* StateMachine::getCurrentState() const {
    return states.empty() ? nullptr : states.back().get();
}

