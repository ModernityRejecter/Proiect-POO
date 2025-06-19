#pragma once
#include <SFML/Graphics.hpp>
#include "../headers/stateMachine.h"
#include "../headers/stateID.h"
#include <SFML/Audio.hpp>

class StateMachine;

class GameState {
public:
    explicit GameState(StateMachine& machine) :
    machine(machine),
    soundBuffer("./assets/sounds/shotgun.wav"),
    sound(soundBuffer) {
        sound.setVolume(20);
    }

    virtual ~GameState() = default;

    virtual void handleEvent(sf::Event* event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual StateID getID() const = 0;

protected:
    StateMachine& machine;
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;
};