#pragma once
#include "../headers/entity.h"
#include "../headers/player.h"

class Imp : public Entity {

private:
    Player* targetPlayer;
    static std::unordered_map<int, std::unordered_map<int, sf::Texture>> entityTextures;

    void idleAnimation() override;

public:
    Imp(sf::Vector2f position, Player* playerPtr);
    void move(float deltaTime) override;
    void update(float deltaTime) override;
    static void loadEntityTextures();
};