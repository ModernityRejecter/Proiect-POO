#include "../headers/hud.h"
#include <SFML/Graphics.hpp>
        void Hud::Section::loadDigitTextures() {

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

        Hud::Section::Section(int amount, float width, float height) : amount(amount), previousAmount(0){
            loadDigitTextures();

            sf::Sprite localSprite(allDigits[0]);

            infoDigits.emplace_back(localSprite);
            infoDigits.emplace_back(localSprite);
            infoDigits.emplace_back(localSprite);

            sectionSetPosition(width, height);
        }
        void Hud::Section::sectionAmmoHandler(int ammo) {
            amount = ammo;
            if (previousAmount != amount) {
                infoDigits[0].setTexture(allDigits[amount/100]);
                infoDigits[1].setTexture(allDigits[(amount/10)%10]);
                infoDigits[2].setTexture(allDigits[amount%10]);


                previousAmount = amount;
            }
        }
        // necesita REVIZIE MASIVA pentru ca e doar un test momentan
        void Hud::Section::sectionSetPosition(float width, float height) {
            infoDigits[0].setPosition(sf::Vector2f(46, height - 192.0f + 48.0f));
            infoDigits[0].setScale(sf::Vector2f(4.0f, 4.0f));
            infoDigits[1].setPosition(sf::Vector2f(110, height - 192.0f + 48.0f));
            infoDigits[1].setScale(sf::Vector2f(4.0f, 4.0f));
            infoDigits[2].setPosition(sf::Vector2f(174, height - 192.0f + 48.0f));
            infoDigits[2].setScale(sf::Vector2f(4.0f, 4.0f));
            std::cout<<"Window width : "<<width<<std::endl;
        }
        void Hud::Section::sectionDraw(sf::RenderWindow& window) const{
            window.draw(infoDigits[0]);
            window.draw(infoDigits[1]);
            window.draw(infoDigits[2]);
        }
    Hud::Hud (const std::string& path, float width, float height, int digits) : texture(path), sprite(texture), ammo(digits, width, height) {
        sprite.setScale(sf::Vector2f(width/static_cast<float>(texture.getSize().x), height/1080.0f));
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(0, height - (height/1080.0f) * static_cast<float>(texture.getSize().y)));
    }
    void Hud::draw(sf::RenderWindow& window) const {
        window.draw(sprite);
        ammo.sectionDraw(window);
    }
    void Hud::hudUpdate(int amount) {
        ammo.sectionAmmoHandler(amount);
    }

    // sf::Texture getTexture() const{
    //     return texture;
    // }
