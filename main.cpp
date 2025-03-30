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

class Projectile {
private:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float lifetime;
    static constexpr float maxLifetime = 3.0f;

public:
    Projectile(const sf::Texture& texture, float startX, float startY, float targetX, float targetY, float speed, float spreadAngle)
        : sprite(texture), speed(speed), lifetime(0.f)
    {
        sf::Vector2u size = texture.getSize();
        sprite.setOrigin(sf::Vector2f(static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f));
        sprite.setPosition(sf::Vector2f(startX, startY));

        sf::Vector2f vec(targetX - startX, targetY - startY);
        float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        direction = (length != 0) ? vec / length : sf::Vector2f(1.f, 0.f);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> angleDist(-spreadAngle / 2, spreadAngle / 2);
        float angleOffset = angleDist(gen); // Unghi de variație

        float radians = angleOffset * (3.14159265f / 180.f);

        float newX = direction.x * std::cos(radians) - direction.y * std::sin(radians);
        float newY = direction.x * std::sin(radians) + direction.y * std::cos(radians);
        direction = sf::Vector2f(newX, newY);
    }

    Projectile (const Projectile& other)
        : sprite(other.sprite), direction(other.direction), speed(other.speed), lifetime(other.lifetime) {
        std::cout<<"Proiectilul a fost copiat cu succes";
    }

    ~Projectile() {
        std::cout<<"Proiectiilul a fost distrus cu succes";
    }

    Projectile& operator=(const Projectile& other) {
        if (this != &other) {
            sprite = other.sprite;
            direction = other.direction;
            speed = other.speed;
            lifetime = other.lifetime;
            // std::cout<<"Copiere efectuata cu succes"<<std::endl; cauzeaza lag
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& info, const Projectile& projectile) {
        info <<"Projectile speed : "<< projectile.speed<<std::endl
             <<"Projectile elapsed lifetime : "<<projectile.lifetime<<std::endl
             <<"Projectile position : ("<<projectile.sprite.getPosition().x <<", "<<projectile.sprite.getPosition().y<<")";
        return info;
    }

    void update(float deltaTime) {
        sprite.move(direction * speed * deltaTime);
        lifetime += deltaTime;
    }

    bool isAlive() const {
        return lifetime < maxLifetime;
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }
};

class Weapon {
private:
    std::string name;
    sf::Texture projectileTexture;
    float fireRate;
    int ammoCount;
    const int maxAmmo;
    const float projectileSpeed;
    sf::Clock fireClock;
    float spread;

public:
    Weapon(const std::string& name, const std::string& projectilePath, float speed, float rate, int ammo, int maxAmmo, float spread)
        : name(name), fireRate(rate), ammoCount(ammo), maxAmmo(maxAmmo), projectileSpeed(speed), spread(spread) {
        if (!projectileTexture.loadFromFile(projectilePath)) {
            std::cerr << "Eroare la incarcarea texturii pentru " << name << "\n";
            exit(EXIT_FAILURE);
        }
    }

    bool canShoot() const {
        return (fireClock.getElapsedTime().asSeconds() >= 1.0f / fireRate && ammoCount != 0);
    }

    void resetFireClock() {
        fireClock.restart();
    }

    Projectile createProjectile(float startX, float startY, float targetX, float targetY) {
        ammoCount -= 1;
        return {projectileTexture, startX, startY, targetX, targetY, projectileSpeed, spread};
    }

    void passiveReload() {
        if (ammoCount < maxAmmo) {}
            ammoCount += 1;
    }
    friend std::ostream& operator<<(std::ostream& info, const Weapon& weapon) {
        info << "Weapon name: " << weapon.name << "\n"
             << "Fire rate: " << weapon.fireRate << "\n"
             << "Projectile speed: " << weapon.projectileSpeed << "\n";
        return info;
    }

    float getFireRate() const {
        return fireRate;
    }
    int getAmmoCount() const {
        return ammoCount;
    }
    int getMaxAmmo() const {
        return maxAmmo;
    }
};

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    sf::Clock interval;
    std::unordered_map<int, std::unordered_map<int, sf::Texture>> playerTextures;
    int textureIndex;
    int directionIndex;
    int previousDirectionIndex;
    std::vector<Weapon> weapons;
    std::vector<Projectile> playerProjectiles;
    size_t currentWeaponIndex = 0;

public:
    Player(const std::string& texturePath, float x, float y, float speed)
        :texture(texturePath), sprite(texture), speed(speed), textureIndex(1), directionIndex(1), previousDirectionIndex(1)
    {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Eroare la incarcarea imaginii jucatorului!\n";
            exit(EXIT_FAILURE);
        }
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(x, y));
        loadWeaponsAttributes();
    }
    // afisarea cauzeaza lag in cazul in care sunt active multe proiectile
    // motiv pentru care este apelata la 5 secunde (trebuie gandita putin mai bine)
    friend std::ostream& operator<<(std::ostream& info, const Player& player) {
        info << "Player position : ("<< player.sprite.getPosition().x<<", "<<player.sprite.getPosition().y<<")"<<std::endl
             << "Current direction : "<<player.directionIndex<<std::endl
             << "Current weapon information: "<<player.weapons[player.currentWeaponIndex]<<std::endl;
        return info;
    }

    void loadPlayerTextures() {
        sf::Texture playerTexture("./assets/textures/player/idle/plr_sprite_d1.png");
        playerTextures[1][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_d2.png");
        playerTextures[1][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sd1.png");
        playerTextures[2][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sd2.png");
        playerTextures[2][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_s1.png");
        playerTextures[3][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_s2.png");
        playerTextures[3][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sa1.png");
        playerTextures[4][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_sa2.png");
        playerTextures[4][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_a1.png");
        playerTextures[5][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_a2.png");
        playerTextures[5][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wa1.png");
        playerTextures[6][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wa2.png");
        playerTextures[6][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_w1.png");
        playerTextures[7][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_w2.png");
        playerTextures[7][2] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wd1.png");
        playerTextures[8][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/idle/plr_sprite_wd2.png");
        playerTextures[8][2] = playerTexture;

        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_s1.png");
        playerTextures[9][1] = playerTexture;
        playerTexture = sf::Texture("./assets/textures/player/shooting/plr_shooting_s2.png");
        playerTextures[9][2] = playerTexture;

    }

    void move(float offsetX, float offsetY) {
        sprite.move({offsetX, offsetY});
    }
    void shootingAnimation() {
        if(static_cast<float>(interval.getElapsedTime().asMilliseconds()) >= 1000.0f / weapons[currentWeaponIndex].getFireRate()) {
            if (textureIndex == 1) {
                sprite.setTexture(playerTextures[9][textureIndex]);
                textureIndex = 2;
            }
            else {
                sprite.setTexture(playerTextures[9][textureIndex]);
                textureIndex = 1;
            }
            interval.restart();
        }
    }
    void loadWeaponsAttributes() {
        weapons.clear();
        weapons.emplace_back("Plasma Rifle", "./assets/textures/projectiles/plasma_proj1.png", 800.0f, 20.0f, 200, 200, 0.0f);
        weapons.emplace_back("BFG", "./assets/textures/projectiles/bfg_proj1.png", 1000.0f, 0.3f, 4, 4, 0.0f);
        weapons.emplace_back("Rocket Launcher", "./assets/textures/projectiles/rocket_proj1.png", 600.0f, 0.7f, 16, 16, 0.0f);
        weapons.emplace_back("Chaingun", "./assets/textures/projectiles/chaingun_proj2.png", 2000.0f, 50.0f, 999, 999, 10.0f);
    }
    void idleAnimation() {
        if (previousDirectionIndex != directionIndex) {
            sprite.setTexture(playerTextures[directionIndex][1]);
        }
        if (interval.getElapsedTime().asMilliseconds() >= 300) {
            if (textureIndex == 1) {
                sprite.setTexture(playerTextures[directionIndex][2]);
                textureIndex = 2;
                // std::cout <<"am actualizat ";
            }
            else {
                sprite.setTexture(playerTextures[directionIndex][1]);
                textureIndex = 1;
                // std::cout <<"am actualizat ";
            }
            // std::cout << textureIndex << std::endl;
            interval.restart();
        }
        // weapons[currentWeaponIndex].passiveReload();
    }

    void update(float deltaTime, unsigned int width, unsigned int height) {

        sf::Vector2f movement(0.f, 0.f);
        sf::Vector2f position = getPosition();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += speed * deltaTime;

        sf::Vector2i mousePos = sf::Mouse::getPosition();
        float angle = std::atan2(static_cast<float>(mousePos.y) - position.y, static_cast<float>(mousePos.x) - position.x) * 180.f / 3.14f;

        previousDirectionIndex = directionIndex;
        if (angle >= -22.5 && angle < 22.5)
            directionIndex = 1;
        else if (angle >= 22.5 && angle < 67.5)
            directionIndex = 2;
        else if (angle >= 67.5 && angle < 112.5)
            directionIndex = 3;
        else if (angle >= 112.5 && angle < 157.5)
            directionIndex = 4;
        else if (angle >= 157.5 || angle < -157.5)
            directionIndex = 5;
        else if (angle >= -157.5 && angle < -112.5)
            directionIndex = 6;
        else if (angle >= -112.5 && angle < -67.5)
            directionIndex = 7;
        else if (angle >= -67.5 && angle < -22.5)
            directionIndex = 8;

        float halfWidth = static_cast<float>(getSize().x) / 2.f;
        float halfHeight = static_cast<float>(getSize().y) / 2.f;

        float minX = 0;
        float maxX = static_cast<float>(width) - 28.f - halfWidth;
        float minY = 0;
        float maxY = static_cast<float>(height) - 28.f - halfHeight - 192.0f;
        sf::Vector2f newPos = position + movement;

        newPos.x = std::max(minX, std::min(newPos.x, maxX));
        newPos.y = std::max(minY, std::min(newPos.y, maxY));
        sprite.setPosition(newPos);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            Weapon& currentWeapon = weapons[currentWeaponIndex];
            if (currentWeapon.canShoot()) {
                shootingAnimation();
                currentWeapon.resetFireClock();

                sf::Vector2f playerCenter = {
                    position.x + static_cast<float>(texture.getSize().x) / 2.0f,
                    position.y + static_cast<float>(texture.getSize().y) / 2.0f
                };

                playerProjectiles.push_back(currentWeapon.createProjectile(
                    playerCenter.x,
                    playerCenter.y,
                    static_cast<float>(mousePos.x),
                    static_cast<float>(mousePos.y)
                ));
            }
        }
        else {
            idleAnimation();
        }

        for (auto& projectile : playerProjectiles) {
            projectile.update(deltaTime);
        }

        std::erase_if(playerProjectiles, [](const Projectile& p) { return !p.isAlive(); });
        weaponsHandler();
    }
    void weaponsHandler() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) currentWeaponIndex = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) currentWeaponIndex = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) currentWeaponIndex = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) currentWeaponIndex = 3;
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
        for (const auto& projectile : playerProjectiles) {
            projectile.draw(window);
        }
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    sf::Vector2u getSize() const {
        return texture.getSize();
    }
    size_t getWeaponIndex() const {
        return currentWeaponIndex;
    }
    int getWeaponAmmoCount() const {
        return weapons[currentWeaponIndex].getAmmoCount();
    }
    int getWeaponMaxAmmo() const {
        return weapons[currentWeaponIndex].getMaxAmmo();
    }
};
class Hud {
    sf::Texture texture;
    sf::Sprite sprite;

    class Section {
        int amount;
        int previousAmount;
        std::vector<sf::Sprite> infoDigits;
        std::vector<sf::Texture> allDigits;

    public:
        void loadDigitTextures() {
            // Redimensionează vectorul pentru a avea 11 elemente (0-9 și un simbol special)
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

        explicit Section(int amount) : amount(amount), previousAmount(0){
            loadDigitTextures();

            sf::Sprite sprite(allDigits[0]);
            if (!allDigits.empty()) {
                infoDigits.emplace_back(sprite);
                infoDigits.emplace_back(sprite);
                infoDigits.emplace_back(sprite);
            }
            sectionSetPosition();
        }
        void sectionAmmoHandler(int ammo) {
            amount = ammo;
            if (previousAmount != amount) {
                infoDigits[0].setTexture(allDigits[amount/100]);
                infoDigits[1].setTexture(allDigits[(amount/10)%10]);
                infoDigits[2].setTexture(allDigits[amount%10]);


                previousAmount = amount;
            }
        }
        // necesita REVIZIE MASIVA pentru ca e doar un test momentan
        void sectionSetPosition() {
            infoDigits[0].setPosition(sf::Vector2f(44, 1200.0f - 192.0f + 48.0f));
            infoDigits[0].setScale(sf::Vector2f(4.0f, 4.0f));
            infoDigits[1].setPosition(sf::Vector2f(108, 1200.0f - 192.0f + 48.0f));
            infoDigits[1].setScale(sf::Vector2f(4.0f, 4.0f));
            infoDigits[2].setPosition(sf::Vector2f(172, 1200.0f - 192.0f + 48.0f));
            infoDigits[2].setScale(sf::Vector2f(4.0f, 4.0f));
        }
        void sectionDraw(sf::RenderWindow& window) const{
            window.draw(infoDigits[0]);
            window.draw(infoDigits[1]);
            window.draw(infoDigits[2]);
        }
    };
    Section ammo;

public:

    Hud (const std::string& path, float height, int digits) : texture(path), sprite(texture), ammo(digits) {
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(0, height - static_cast<float>(texture.getSize().y)));
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
        ammo.sectionDraw(window);
    }
    void hudUpdate(int amount) {
        ammo.sectionAmmoHandler(amount);
    }

    sf::Texture getTexture() const{
        return texture;
    }
};
class Game {
private:
    sf::RenderWindow window;
    Player player;
    bool shouldExit = false;
    sf::Music soundTrack;
    std::vector<std::string> trackPaths;
    float musicVolume;
    int currentMusicIndex;
    Hud hud;

public:
    Game()
        : window(sf::VideoMode::getDesktopMode(), "ETERNAL DOOM", sf::State::Fullscreen),
          player("./assets/textures/player/idle/plr_sprite_s1.png", 400.f, 300.f, 300.f),
          musicVolume(20.0f), currentMusicIndex(0), hud("./assets/textures/hud/hud_bg.png", static_cast<float>(window.getSize().y), 0){
        window.setVerticalSyncEnabled(true);
    }

    void loadTracks() {
        trackPaths.emplace_back("./assets/music/RTPN-Uprizing-_-y_5KVimqxI_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Sustain-_Si1Uw_nmjMk_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Release-_9eluFZIpmOs_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Hive-_u2lNDShmM-k_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-ClownIsWe-_O_t53q1vq04_.ogg");
        trackPaths.emplace_back("./assets/music/RTPN-Decay-_kK2k_W0nKdE_.ogg");
    }

    void run() {
        player.loadPlayerTextures();
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
    friend std::ostream& operator<<(std::ostream& info, const Game &game) {
        info <<"Player information : "<< game.player <<std::endl;
        return info;
    }
private:
    void processEvents() {
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
    void update(float deltaTime) {
        player.update(deltaTime, window.getSize().x, window.getSize().y);
        hud.hudUpdate(player.getWeaponAmmoCount());
    }

    void render() {
        window.clear();
        player.draw(window);
        hud.draw(window);
        window.display();
    }

    void musicHandler () {
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
};

int main() {
    Game game;
    game.run();
    return 0;
}