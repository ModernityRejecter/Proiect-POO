#include "../headers/mainMenuState.h"
#include "../headers/stateMachine.h"

MainMenuState::MainMenuState(StateMachine& m)
: GameState(m), font("./assets/fonts/AmazDooMLeft.ttf"), title(font, "you should love yourself NOW!!!", 30)
{
    // std::ignore = font.openFromFile("./assets/fonts/arial.ttf");
//    title.setFont(font);
//    title.setString("Main Menu\nPress Enter to Play");
//    title.setCharacterSize(30);
    title.setPosition({100, 100});
}

void MainMenuState::handleEvent(sf::Event* event) {
    if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::Enter) {
        machine.change(StateID::Play);
    }
}

void MainMenuState::update(float deltaTime) {
    //WIP
}

void MainMenuState::draw(sf::RenderWindow& window) {
    window.draw(title);
}

StateID MainMenuState::getID() const {
    return StateID::MainMenu;
}