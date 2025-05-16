#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include<cstdlib>
class Hud {
    sf::FloatRect rect;
    int amount;
    int previousAmount;
    std::vector<sf::Sprite> infoDigits;
    static std::vector<sf::Texture> allDigits;
    static constexpr float scaleRatio = 6.0f;

    void setDigitPosition();
    void valueHandler(int value);
    static void loadDigitTextures();
public:

    Hud(int amount, sf::Vector2f position, sf::Vector2f size);
    void hudUpdate(int value);
    void draw(sf::RenderWindow& window) const;
    // void hudDraw(sf::RenderWindow& window) const;
};