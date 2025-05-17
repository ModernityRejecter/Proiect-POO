#include "../headers/pauseState.h"
#include "../headers/stateMachine.h"

static constexpr float LOGICAL_WIDTH  = 1920;

PauseState::PauseState(StateMachine& m)
    : GameState(m), font("./assets/fonts/AmazDooMLeft.ttf"), pauseText(font, "we have sent unto them...\nonly you. rip and tear,\n until it is done", 200),
      instructionFont("./assets/fonts/AmazDooMRight.ttf"), instruction(instructionFont, "press [ p ] to return to your fight", 100),
      pauseTexture("./assets/textures/backgrounds/pause_menu.png"), pauseSprite(pauseTexture)
{
    // std::ignore = font.openFromFile("./assets/fonts/arial.ttf");
//    pauseText.setFont(font);
//    pauseText.setString("Paused\nPress P to Resume");
//    pauseText.setCharacterSize(30);
    pauseText.setPosition({(LOGICAL_WIDTH / 2) - pauseText.getLocalBounds().size.x / 2, 100});
    pauseText.setOutlineColor(sf::Color::Black);
    pauseText.setOutlineThickness(3);
    pauseSprite.setPosition({0,0});
    instruction.setPosition({(LOGICAL_WIDTH / 2) - instruction.getLocalBounds().size.x / 2, pauseText.getLocalBounds().size.y + 200});
    instruction.setOutlineColor(sf::Color::Black);
    instruction.setOutlineThickness(3);
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
    window.draw(pauseSprite);
    window.draw(pauseText);
    window.draw(instruction);
}

StateID PauseState::getID() const {
    return StateID::Pause;
}