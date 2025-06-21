#include "../headers/gameOverState.h"

GameOverState::GameOverState(StateMachine& machine)
    : GameState(machine),
      backgroundTexture("./assets/textures/backgrounds/game_over.png"),
      backgroundSprite(backgroundTexture),
      fontLeft("./assets/fonts/AmazDooMLeft.ttf"),
      fontRight("./assets/fonts/AmazDooMRight.ttf"),
      textLeft(fontLeft, "Game", 300),
      textRight(fontRight, "oveR", 300),
      score(fontLeft, "score: ", 300),
      instruction(fontLeft, "press [ enter ] to rise from the depths of hell\npress [ esc ] to accept your eternal damnation", 100)

{
    reset();
    backgroundSprite.setPosition({0, 0});

    textLeft.setOutlineColor(sf::Color::Black);
    textLeft.setOutlineThickness(3);
    textLeft.setPosition(
        {(LOGICAL_WIDTH / 2 - textLeft.getLocalBounds().size.x - 40.f),
        100.f}
    );

    textRight.setOutlineColor(sf::Color::Black);
    textRight.setOutlineThickness(3);
    textRight.setPosition(
        { (LOGICAL_WIDTH / 2 + 40.f),
        100.f}
    );

    score.setOutlineColor(sf::Color::Black);
    score.setOutlineThickness(3);
    score.setPosition(
        {(LOGICAL_WIDTH - score.getLocalBounds().size.x) / 2.f,
        500.f}
    );

    instruction.setOutlineColor(sf::Color::Black);
    instruction.setOutlineThickness(2);
    instruction.setPosition(
        {(LOGICAL_WIDTH - instruction.getLocalBounds().size.x) / 2.f,
        850.f}
    );
}

void GameOverState::handleEvent(sf::Event* event) {
    if (event->is<sf::Event::KeyPressed>()) {
        auto code = event->getIf<sf::Event::KeyPressed>()->scancode;
        if (code == sf::Keyboard::Scan::Enter) {
            sound.play();
            Game::getInstance().reset();
            machine.change(StateID::MainMenu);
        }
        else if (code == sf::Keyboard::Scan::Escape) {
            std::exit(0);
        }
    }
}

void GameOverState::update(float /*deltaTime*/) {
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

void GameOverState::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(textLeft);
    window.draw(textRight);
    window.draw(score);
    window.draw(instruction);
}

StateID GameOverState::getID() const {
    return StateID::GameOver;
}

void GameOverState::reset() {
    score.setString("score: " + std::to_string(Game::getInstance().getScore()));
}