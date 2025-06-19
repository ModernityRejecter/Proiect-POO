#include "../headers/playState.h"
#include "../headers/stateMachine.h"

PlayState::PlayState(StateMachine& m)
: GameState(m) {}

void PlayState::handleEvent(sf::Event* event) {
    if (event->is<sf::Event::KeyPressed>()) {
        const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->scancode == sf::Keyboard::Scan::P && dynamic_cast<PlayState*>(machine.getCurrentState())->getID() == StateID::Play) {
            sound.play();
            machine.change(StateID::Pause);
        }
    }
}

void PlayState::update([[maybe_unused]] float deltaTime) {
}

void PlayState::draw([[maybe_unused]] sf::RenderWindow& window) {
}

StateID PlayState::getID() const {
    return StateID::Play;
}