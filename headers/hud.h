#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include<cstdlib>
class Hud {
    sf::Texture texture;
    sf::Sprite sprite;

    class Section {
        int amount;
        int previousAmount;
        std::vector<sf::Sprite> infoDigits;
        std::vector<sf::Texture> allDigits;
    public:
        void loadDigitTextures();
        explicit Section(int amount, float width, float height);
        void sectionAmmoHandler(int ammo);
        // necesita REVIZIE MASIVA pentru ca e doar un test momentan
        void sectionSetPosition(float width, float height);
        void sectionDraw(sf::RenderWindow& window) const;
    };
    Section ammo;
public:
    Hud (const std::string& path, float width, float height, int digits);
    void draw(sf::RenderWindow& window) const;
    void hudUpdate(int amount);
    // sf::Texture getTexture() const;
};