#include "../headers/game.h"

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "ETERNAL DOOM", sf::State::Fullscreen),
      shouldExit(false),
      musicVolume(20.0f),
      currentMusicIndex(0),
      background("./assets/textures/backgrounds/bg1.png"),
      backgroundSprite(background),
      hudTexture("./assets/textures/hud/hud_bg.png"),
      hudSprite(hudTexture),
      view(sf::FloatRect({0,0},{LOGICAL_WIDTH,LOGICAL_HEIGHT})),
      ammo(0, sf::Vector2f(0, getGlobalBounds().y), sf::Vector2f(282,192)),
      health(0, sf::Vector2f(288, getGlobalBounds().y), sf::Vector2f(342,192)),
      armor(0, sf::Vector2f(1068, getGlobalBounds().y), sf::Vector2f(348,192)),
      enemySpawner(entities)
{
    Player::loadEntityTextures();
    Imp::loadEntityTextures();

    window.setView(view);
    onResize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    window.setVerticalSyncEnabled(true);

    states.change(StateID::Pause);
    states.change(StateID::Play);
    states.change(StateID::MainMenu);

    backgroundSprite.setPosition({0,0});
    hudSprite.setPosition({0, LOGICAL_HEIGHT - static_cast<float>(hudTexture.getSize().y)});
    entities.reserve(8);

    auto playerPtr = std::make_shared<Player>(
        "./assets/textures/player/idle/sprite_1_1.png",
        400.f, 300.f, 300.f
    );
    entities.push_back(playerPtr);

    ammo.hudUpdate(playerPtr->getWeaponMaxAmmo());
    health.hudUpdate(playerPtr->getHealth());
    armor.hudUpdate(playerPtr->getPlayerArmor());

    enemySpawner.init(playerPtr, getGlobalBounds());
}

void Game::loadTracks() {
    trackPaths.emplace_back("./assets/music/RTPN-Uprizing-_-y_5KVimqxI_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Sustain-_Si1Uw_nmjMk_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Release-_9eluFZIpmOs_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Hive-_u2lNDShmM-k_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-ClownIsWe-_O_t53q1vq04_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Decay-_kK2k_W0nKdE_.ogg");
}

sf::Vector2f Game::getGlobalBounds() const {
    return { LOGICAL_WIDTH,
             LOGICAL_HEIGHT - static_cast<float>(hudTexture.getSize().y) };
}

void Game::onResize(float w, float h) {
    float targetRatio = LOGICAL_WIDTH / LOGICAL_HEIGHT;
    float windowRatio = w / h;

    float viewportW = 1.f, viewportH = 1.f;
    float offsetX = 0.f, offsetY = 0.f;

    if (windowRatio > targetRatio) {
        viewportW = targetRatio / windowRatio;
        offsetX = (1.f - viewportW) / 2.f;
    } else {
        viewportH = windowRatio / targetRatio;
        offsetY = (1.f - viewportH) / 2.f;
    }

    view.setViewport(sf::FloatRect({offsetX, offsetY}, {viewportW, viewportH}));
    window.setView(view);
}

void Game::run() {
    loadTracks();
    sf::Clock clock;
    sf::Clock infoClock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();

        if (infoClock.getElapsedTime().asMilliseconds() >= 5000) {
            if (!entities.empty()) {
                if (auto playerPtr = std::dynamic_pointer_cast<Player>(entities[0])) {
                    std::cout << *playerPtr << std::endl;
                }
            }
            infoClock.restart();
        }
    }
}

std::ostream& operator<<(std::ostream& info, const Game &game) {
    if (!game.entities.empty()) {
        if (auto playerPtr = std::dynamic_pointer_cast<Player>(game.entities[0])) {
            info << "Player information : " << *playerPtr << std::endl;
        }
    }
    return info;
}

void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        states.handleEvent(*event);
        musicHandler(&(*event));
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::Escape)
        {
            shouldExit = true;
        }
        if (event->is<sf::Event::Resized>()) {
            auto re = event->getIf<sf::Event::Resized>();
            onResize(static_cast<float>(re->size.x), static_cast<float>(re->size.y));
        }
    }
    if (shouldExit) window.close();
}

void Game::update(float deltaTime) {
    states.update(deltaTime);

    if (states.getCurrentState()->getID() == StateID::Play) {
        if (!entities.empty()) {
            if (auto playerPtr = std::dynamic_pointer_cast<Player>(entities[0])) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                playerPtr->setAim(worldPos);

                ammo.hudUpdate(playerPtr->getWeaponAmmoCount());
                health.hudUpdate(playerPtr->getHealth());
                armor.hudUpdate(playerPtr->getPlayerArmor());
            }
        }

        for (const auto& ent : entities) {
            ent->update(deltaTime);
        }

        for (auto& ent : entities) {
            auto& projList = ent->getProjectiles();
            for (auto& p : projList) {
                if (!p->isAlive()) continue;

                if (p->getBounds().position.y > LOGICAL_HEIGHT - 200) {
                    p->deactivate();
                    continue;
                }

                for (const auto& targetEnt : entities) {
                    auto ownerPtr = p->getOwner();
                    if (ownerPtr && ownerPtr.get() == targetEnt.get()) {
                        continue;
                    }
                    if (!targetEnt->isAlive()) continue;
                    if (p->getBounds().findIntersection(targetEnt->getBounds())) {
                        targetEnt->takeDamage(p->getDamage());
                        p->deactivate();
                        break;
                    }
                }
            }
        }
        std::erase_if(
            entities,
            [](const std::shared_ptr<Entity>& e) {
                return !e->isAlive();
            }
        );
        enemySpawner.update(deltaTime);
    }
}

void Game::render() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(hudSprite);
    for (const auto& ent : entities) {
        ent->draw(window);
        for (const auto& p : ent->getProjectiles()) {
            p->draw(window);
        }
    }
    ammo.draw(window);
    health.draw(window);
    armor.draw(window);
    sf::Texture faceTexture("./assets/textures/player/faces/face1.png");
    sf::Sprite faceSprite(faceTexture);
    faceSprite.setPosition({
        LOGICAL_WIDTH / 2.f
          - (faceTexture.getSize().x / 2.f) * (3.f * LOGICAL_WIDTH / LOGICAL_HEIGHT),
        LOGICAL_HEIGHT
          - 192.f / 2.f
          - ((faceTexture.getSize().y * 3.f * LOGICAL_WIDTH / LOGICAL_HEIGHT) / 2.f)
          + 6.f
    });
    faceSprite.setScale({3.f * LOGICAL_WIDTH / LOGICAL_HEIGHT,
                         3.f * LOGICAL_WIDTH / LOGICAL_HEIGHT});
    window.draw(faceSprite);
    states.draw(window);
    window.display();
}


void Game::musicHandler(const sf::Event* event) {
    if (window.hasFocus()) {
        if (event->is<sf::Event::KeyReleased>() &&
            event->getIf<sf::Event::KeyReleased>()->scancode == sf::Keyboard::Scan::M &&
            soundTrack.getStatus() == sf::SoundSource::Status::Playing)
        {
            std::cout << "Playback paused" << std::endl;
            soundTrack.pause();
        }
        else if (event->is<sf::Event::KeyReleased>() &&
                 event->getIf<sf::Event::KeyReleased>()->scancode == sf::Keyboard::Scan::M &&
                 soundTrack.getStatus() == sf::SoundSource::Status::Paused)
        {
            std::cout << "Playback resumed" << std::endl;
            soundTrack.play();
        }
        if (event->is<sf::Event::KeyReleased>() &&
            event->getIf<sf::Event::KeyReleased>()->scancode == sf::Keyboard::Scan::Hyphen &&
            soundTrack.getStatus() == sf::SoundSource::Status::Playing &&
            soundTrack.getVolume() > 0.f)
        {
            std::cout << "Volume lowered" << std::endl;
            musicVolume -= 2.f;
            soundTrack.setVolume(musicVolume);
        }
        if (event->is<sf::Event::KeyReleased>() &&
            event->getIf<sf::Event::KeyReleased>()->scancode == sf::Keyboard::Scan::Equal &&
            soundTrack.getStatus() == sf::SoundSource::Status::Playing &&
            soundTrack.getVolume() < 30.f)
        {
            std::cout << "Volume increased" << std::endl;
            musicVolume += 2.f;
            soundTrack.setVolume(musicVolume);
        }
    }
    if (soundTrack.getStatus() == sf::Sound::Status::Stopped) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, static_cast<int>(trackPaths.size()) - 1);
        currentMusicIndex = distrib(gen);
        std::cout << "Now Playing :" << currentMusicIndex << std::endl;
        std::ignore = soundTrack.openFromFile(trackPaths[currentMusicIndex]);
        soundTrack.play();
        soundTrack.setVolume(musicVolume);
    }
}
