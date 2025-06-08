#include <iostream>
#include <vector>
#include <complex>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio/Music.hpp>
#include <unordered_map>
#include <cmath>
#include <cstdlib>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio/Sound.hpp>
#include <random>
#include "headers/game.h"

int main() {
    try {
        Game::getInstance().run();
    }
    catch (std::exception& exception) {
        std::cout << exception.what() << std::endl;
    }
    return 0;
}