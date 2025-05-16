#include "../headers/game.h"

static constexpr float LOGICAL_WIDTH  = 1920;
static constexpr float LOGICAL_HEIGHT = 1080;

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "ETERNAL DOOM", sf::State::Fullscreen),
        player("./assets/textures/player/idle/plr_sprite_s1.png", 400.f, 300.f, 300.f),
        musicVolume(20.0f), currentMusicIndex(0), background("./assets/textures/backgrounds/bg1.png"), backgroundSprite(background),
        hudTexture("../assets/textures/hud/hud_bg.png"), hudSprite(hudTexture),
        view(sf::FloatRect({0,0},{LOGICAL_WIDTH, LOGICAL_HEIGHT})),
        ammo(player.getWeaponMaxAmmo(), sf::Vector2f(0, getGlobalBounds().y), sf::Vector2f(282, 192)),
        health(player.getHealth(), sf::Vector2f(288, getGlobalBounds().y), sf::Vector2f(342, 192)),
        armor(player.getPlayerArmor(), sf::Vector2f{1068, getGlobalBounds().y}, sf::Vector2f{348, 192})
        /*, hud("./assets/textures/hud/hud_bg.png",static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y), 0)*/{
    window.setView(view);
    onResize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    window.setVerticalSyncEnabled(true);
    backgroundSprite.setPosition({0,0});
    hudSprite.setPosition({0,LOGICAL_HEIGHT - static_cast<float>(hudTexture.getSize().y)});
}

void Game::loadTracks() {
    trackPaths.emplace_back("./assets/music/RTPN-Uprizing-_-y_5KVimqxI_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Sustain-_Si1Uw_nmjMk_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Release-_9eluFZIpmOs_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Hive-_u2lNDShmM-k_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-ClownIsWe-_O_t53q1vq04_.ogg");
    trackPaths.emplace_back("./assets/music/RTPN-Decay-_kK2k_W0nKdE_.ogg");
}

sf::Vector2f Game::getGlobalBounds() const{
    return sf::Vector2f(LOGICAL_WIDTH, LOGICAL_HEIGHT - static_cast<float>(hudTexture.getSize().y));
}

void Game::onResize(float w, float h) {
    float targetRatio = LOGICAL_WIDTH / LOGICAL_HEIGHT;
    float windowRatio = w / h;

    float viewportW = 1.f;
    float viewportH = 1.f;
    float offsetX  = 0.f;
    float offsetY  = 0.f;

    if (windowRatio > targetRatio) {
        viewportW = targetRatio / windowRatio;
        offsetX   = (1.f - viewportW) / 2.f;
    }
    else {
        viewportH = windowRatio / targetRatio;
        offsetY   = (1.f - viewportH) / 2.f;
    }

    view.setViewport(sf::FloatRect({ offsetX, offsetY}, {viewportW, viewportH }));
    window.setView(view);
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
        if (event->is<sf::Event::Resized>()) {
            auto re = event->getIf<sf::Event::Resized>();
            onResize(static_cast<float>(re->size.x), static_cast<float>(re->size.y));
            // ammo.setDigitPosition();
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
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    player.setAim(worldPos);

    player.update(deltaTime);
    int ammoCount = player.getWeaponAmmoCount();
    ammo.hudUpdate(ammoCount);
    int playerHealth = player.getHealth();
    health.hudUpdate(playerHealth);
    int playerArmor = player.getPlayerArmor();
    armor.hudUpdate(playerArmor);
}

void Game::render() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(hudSprite);
    player.draw(window);
    ammo.draw(window);
    health.draw(window);
    armor.draw(window);
    sf::Texture faceTexture("./assets/textures/player/faces/face1.png"); // mic extra ca decoratiune momentan
    sf::Sprite faceSprite(faceTexture);
    faceSprite.setPosition({LOGICAL_WIDTH / 2 - faceTexture.getSize().x / 2 * 3 * LOGICAL_WIDTH / LOGICAL_HEIGHT, LOGICAL_HEIGHT - 192 / 2 - (faceTexture.getSize().y * 3 * LOGICAL_WIDTH / LOGICAL_HEIGHT) / 2 + 6});
    faceSprite.setScale({3 * LOGICAL_WIDTH / LOGICAL_HEIGHT, 3 * LOGICAL_WIDTH / LOGICAL_HEIGHT}); // placeholder, deocamdata e implementat intr-un mod foarte stupid
    window.draw(faceSprite);
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
