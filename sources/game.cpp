#include "../headers/game.h"

    Game::Game()
        : window(sf::VideoMode::getDesktopMode(), "ETERNAL DOOM", sf::State::Fullscreen),
          player("./assets/textures/player/idle/plr_sprite_s1.png", 400.f, 300.f, 300.f),
          musicVolume(20.0f), currentMusicIndex(0), background("./assets/textures/backgrounds/bg1.png"), backgroundSprite(background), hud("./assets/textures/hud/hud_bg.png",static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y), 0){
        window.setVerticalSyncEnabled(true);
        backgroundSprite.setPosition({0,0});
    }

    void Game::loadTracks() {
        trackPaths.emplace_back("./assets/music/RTPN-Uprizing-_-y_5KVimqxI_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Sustain-_Si1Uw_nmjMk_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Release-_9eluFZIpmOs_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Hive-_u2lNDShmM-k_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-ClownIsWe-_O_t53q1vq04_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Decay-_kK2k_W0nKdE_.ogg");
    }

    void Game::run() {
        player.loadEntityTextures();
        loadTracks();
        sf::Clock clock;
        sf::Clock informationClock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            processEvents();
            update(deltaTime);
            musicHandler();
            render();
            if (informationClock.getElapsedTime().asMilliseconds() >= 5000) {
                std::cout<<player;
                informationClock.restart();
            }
        }
    }
    std::ostream& operator<<(std::ostream& info, const Game &game) {
        info <<"Player information : "<< game.player <<std::endl;
        return info;
    }

    void Game::processEvents() {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::Escape) {
                shouldExit = true;
            }
        }
        if (shouldExit) window.close();
    }
    // bool isWithinBounds() {
    //     if (player.getPosition().x > 20 && player.getPosition().y > 20 &&
    //         player.getPosition().x < window.getSize().x - 20 && player.getPosition().y < window.getSize().y - 20) {
    //         return true;
    //     }
    // }
    void Game::update(float deltaTime) {
        player.update(deltaTime, window.getSize().x, window.getSize().y);
        hud.hudUpdate(player.getWeaponAmmoCount());
    }

    void Game::render() {
        window.clear();
        window.draw(backgroundSprite);
        player.draw(window);
        hud.draw(window);
        window.display();
    }

    void Game::musicHandler () {
        static sf::Clock clock;
        if (clock.getElapsedTime().asMilliseconds() >= 300 && window.hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen) && soundTrack.getVolume() > 0) {
                std::cout<<"Volume lowered"<<std::endl;
                musicVolume -= 1;
                soundTrack.setVolume(musicVolume);
                clock.restart();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal) && soundTrack.getVolume() < 30) {
                std::cout<<"Volume increased"<<std::endl;
                musicVolume += 1;
                soundTrack.setVolume(musicVolume);
                clock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M) && soundTrack.getVolume() != 0) {
                soundTrack.setVolume(0);
                clock.restart();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M) && soundTrack.getVolume() == 0) {
                soundTrack.setVolume(musicVolume);
                clock.restart();
            }
        }
        if (soundTrack.getStatus() == sf::Sound::Status::Stopped) {
            std::random_device random;
            std::mt19937 gen(random());
            std::uniform_int_distribution<> distrib(0, static_cast<int>(trackPaths.size()) - 1);
            currentMusicIndex = distrib(gen);
            std::cout<<"Now Playing :"<<currentMusicIndex<<std::endl;
            std::ignore = soundTrack.openFromFile(trackPaths[currentMusicIndex]);
            soundTrack.play();
            soundTrack.setVolume(musicVolume);
        }
    }
