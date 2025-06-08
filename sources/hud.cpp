#include "../headers/hud.h"
#include <SFML/Graphics.hpp>
std::vector<sf::Texture> Hud::allDigits;
void Hud::loadDigitTextures() {

    allDigits.resize(11);
    if(!allDigits[0].loadFromFile("./assets/textures/hud/b0.png"))
        std::cerr << "Eroare la incarcarea b0.png\n";
    if(!allDigits[1].loadFromFile("./assets/textures/hud/b1.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[2].loadFromFile("./assets/textures/hud/b2.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[3].loadFromFile("./assets/textures/hud/b3.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[4].loadFromFile("./assets/textures/hud/b4.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[5].loadFromFile("./assets/textures/hud/b5.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[6].loadFromFile("./assets/textures/hud/b6.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[7].loadFromFile("./assets/textures/hud/b7.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[8].loadFromFile("./assets/textures/hud/b8.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[9].loadFromFile("./assets/textures/hud/b9.png"))
        std::cerr << "Eroare la incarcarea b1.png\n";
    if(!allDigits[10].loadFromFile("./assets/textures/hud/b%.png"))
        std::cerr << "Eroare la incarcarea b%.png\n";
}

Hud::Hud(int amount, sf::Vector2f position, sf::Vector2f size) :  rect(position, size), amount(amount), previousAmount(0) {
    loadDigitTextures();

    sf::Sprite localSprite(allDigits[0]);

    infoDigits.emplace_back(localSprite);
    infoDigits.emplace_back(localSprite);
    infoDigits.emplace_back(localSprite);

    setDigitPosition();

}
void Hud::valueHandler(int value) {
        infoDigits[0].setTexture(allDigits[value/100]);
        infoDigits[1].setTexture(allDigits[(value/10)%10]);
        infoDigits[2].setTexture(allDigits[value%10]);
}

void Hud::setDigitPosition() {
    infoDigits[0].setPosition(sf::Vector2f(rect.position.x + (rect.size.x - 56*scaleRatio*0.25*3 - 14) / 2, rect.position.y + rect.size.y / 2 - 56*1.2 ));
    infoDigits[0].setScale(sf::Vector2f(scaleRatio, scaleRatio));
    infoDigits[1].setPosition(sf::Vector2f(infoDigits[0].getPosition().x + 56*scaleRatio*0.25, rect.position.y + rect.size.y / 2 - 56*1.2 ));
    infoDigits[1].setScale(sf::Vector2f(scaleRatio, scaleRatio));
    infoDigits[2].setPosition(sf::Vector2f(infoDigits[1].getPosition().x + 56*scaleRatio*0.25 , rect.position.y + rect.size.y / 2 - 56*1.2 ));
    infoDigits[2].setScale(sf::Vector2f(scaleRatio, scaleRatio));
}

void Hud::draw(sf::RenderWindow& window) const {
    for (auto& digit: infoDigits)
        window.draw(digit);
}

void Hud::hudUpdate(int value) {
    amount = value;
    if (previousAmount != amount)
        valueHandler(amount);

    previousAmount = amount;
}
