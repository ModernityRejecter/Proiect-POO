#include "../headers/playState.h"
#include "../headers/stateMachine.h"

PlayState::PlayState(StateMachine& m)
: GameState(m)
{
    // load level, initialize player etc.
}

void PlayState::handleEvent(sf::Event* event) {
    if (event->is<sf::Event::KeyPressed>()) {
        const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->scancode == sf::Keyboard::Scan::P && dynamic_cast<PlayState*>(machine.getCurrentState())->getID() == StateID::Play) {
            machine.change(StateID::Pause);
        }
    }
}

void PlayState::update(float deltaTime) {
    // game logic
}

void PlayState::draw(sf::RenderWindow& window) {
    // draw game world
}

StateID PlayState::getID() const {
    return StateID::Play;
}