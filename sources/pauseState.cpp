#include "../headers/pauseState.h"
#include "../headers/stateMachine.h"

PauseState::PauseState(StateMachine& m)
: GameState(m), font("./assets/fonts/AmazDooMLeft.ttf"), pauseText(font, "ayo i got a pizza here", 30)
{
    // std::ignore = font.openFromFile("./assets/fonts/arial.ttf");
//    pauseText.setFont(font);
//    pauseText.setString("Paused\nPress P to Resume");
//    pauseText.setCharacterSize(30);
    pauseText.setPosition({500, 100});
}

void PauseState::handleEvent(sf::Event* event) {
    if (event->is<sf::Event::KeyPressed>()) {
        const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->scancode == sf::Keyboard::Scan::P && dynamic_cast<PauseState*>(machine.getCurrentState())->getID() == StateID::Pause) {
            machine.change(StateID::Play);
        }
    }
}

void PauseState::update([[maybe_unused]] float deltaTime) {
    //WIP
}

void PauseState::draw(sf::RenderWindow& window) {
    window.draw(pauseText);
}

StateID PauseState::getID() const {
    return StateID::Pause;
}