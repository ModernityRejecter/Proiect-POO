#include "../headers/gameOverState.h"

GameOverState::GameOverState(StateMachine& machine)
    : GameState(machine),
      backgroundTexture("./assets/textures/backgrounds/game_over.png"),
      backgroundSprite(backgroundTexture),
      font("./assets/fonts/AmazDooMLeft.ttf"),
      title(font, "GAME OVER", 300),
      score(font, "score: " + std::to_string(Game::getInstance().getScore()), 300),
      instruction(font, "press [ enter ] or [ esc ] to accept your eternal damnation", 100)
{
    backgroundSprite.setPosition({0, 0});
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3);
    title.setPosition(
        {(LOGICAL_WIDTH - title.getLocalBounds().size.x)  / 2.f,
        100.f}
    );

    score.setOutlineColor(sf::Color::Black);
    score.setOutlineThickness(2);
    score.setPosition(
        {(LOGICAL_WIDTH - score.getLocalBounds().size.x)  / 2.f,
        500.f}
    );

    instruction.setOutlineColor(sf::Color::Black);
    instruction.setOutlineThickness(2);
    instruction.setPosition(
        {(LOGICAL_WIDTH - instruction.getLocalBounds().size.x)  / 2.f,
        900.f}
    );
}

void GameOverState::handleEvent(sf::Event* event) {
    if (event->is<sf::Event::KeyPressed>()) {
        auto code = event->getIf<sf::Event::KeyPressed>()->scancode;
        if (code == sf::Keyboard::Scan::Enter || code == sf::Keyboard::Scan::Escape) {
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
    window.draw(title);
    window.draw(score);
    window.draw(instruction);
}

StateID GameOverState::getID() const {
    return StateID::GameOver;
}
