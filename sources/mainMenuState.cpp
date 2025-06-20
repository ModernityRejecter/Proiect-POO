#include "../headers/mainMenuState.h"

static constexpr float LOGICAL_WIDTH  = 1920;

MainMenuState::MainMenuState(StateMachine& m)
    : GameState(m), fontLeft("./assets/fonts/AmazDooMLeft.ttf"), fontRight("./assets/fonts/AmazDooMRight.ttf"),
      titleLeft(fontLeft, "Do", 400),
      titleLeftExtra(fontLeft, "Eter", 400),
      titleRight(fontRight, "oM", 400),
      titleRightExtra(fontRight, "naL", 400),
      menuTexture("./assets/textures/backgrounds/main_menu.png"), menuSprite(menuTexture),
      instruction(fontLeft, "press [ enter ] to start purging the forces of hell", 100) {
    // std::ignore = font.openFromFile("./assets/fonts/arial.ttf");
    //    title.setFont(font);
    //    title.setString("Main Menu\nPress Enter to Play");
    //    title.setCharacterSize(30);
    titleLeft.setPosition({(LOGICAL_WIDTH / 2) - titleLeft.getLocalBounds().size.x, 340});
    titleRight.setPosition({(LOGICAL_WIDTH / 2) , 340});
    titleLeftExtra.setPosition({(LOGICAL_WIDTH / 2) - titleLeftExtra.getLocalBounds().size.x + 67, 30});
    titleRightExtra.setPosition({(LOGICAL_WIDTH / 2) + 67, 30});
    titleLeft.setOutlineColor(sf::Color::Black);
    titleRight.setOutlineColor(sf::Color::Black);
    titleLeftExtra.setOutlineColor(sf::Color::Black);
    titleRightExtra.setOutlineColor(sf::Color::Black);
    titleLeft.setOutlineThickness(3);
    titleRight.setOutlineThickness(3);
    titleLeftExtra.setOutlineThickness(3);
    titleRightExtra.setOutlineThickness(3);
    instruction.setPosition({(LOGICAL_WIDTH / 2) - instruction.getLocalBounds().size.x / 2, titleLeft.getLocalBounds().size.y + 600});
    instruction.setOutlineColor(sf::Color::Black);
    instruction.setOutlineThickness(3);
    menuSprite.setPosition({0, 0});
    clock.restart();
}

void MainMenuState::handleEvent(sf::Event* event) {
    if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::Enter) {
        std::ignore = menuTexture.loadFromFile("./assets/textures/backgrounds/main_menu.png");
        menuSprite.setTexture(menuTexture);
        sound.play();
        machine.change(StateID::Play);
    }
}

void MainMenuState::update([[maybe_unused]] float deltaTime) {
    if (clock.getElapsedTime().asMilliseconds() > 700) {
        std::uint8_t currentAlpha = instruction.getFillColor().a;
        std::uint8_t newAlpha;
        if (currentAlpha == 255)
            newAlpha = 0;
        else
            newAlpha = 255;
        instruction.setFillColor(sf::Color(255, 255, 255, newAlpha));
        instruction.setOutlineColor(sf::Color(0, 0, 0, newAlpha));
        clock.restart();
    }
}

void MainMenuState::draw(sf::RenderWindow& window) {
    window.draw(menuSprite);
    window.draw(titleLeft);
    window.draw(titleRight);
    window.draw(titleLeftExtra);
    window.draw(titleRightExtra);
    window.draw(instruction);
}

StateID MainMenuState::getID() const {
    return StateID::MainMenu;
}