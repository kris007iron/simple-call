#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include "map/Map.hpp"
#include "map/map_renderer/MapRenderer.hpp"
#include "enemy/EnemyManager.hpp"
#include "map/exit_renderer/ExitRenderer.hpp"
#include "map/respawn_renderer/RespawnRenderer.hpp"

enum class GameStateID {
    MENU,
    GAMEPLAY,
    SETTINGS,
    END_GAME,
    PAUSE_MENU,
};

struct GameState {
    void (*handleEvent)(const sf::Event&, sf::RenderWindow&);
    void (*update)(sf::Time, sf::RenderWindow&);
    void (*draw)(sf::RenderWindow&);
};


struct Slider {
    sf::RectangleShape bar;
    sf::CircleShape knob;
    float minX;
    float maxX;
    float value;
    bool dragging;
};

struct ScoreEntry {
    std::string playerName;
    int score;
};

struct Bullet
{
    sf::Sprite sprite;
    sf::Vector2f velocity;

    Bullet(const sf::Texture& tex,
        sf::Vector2f position,
        sf::Vector2f vel)
        : sprite(tex), velocity(vel)
    {
        sprite.setOrigin(
            { tex.getSize().x / 2.f,
            tex.getSize().y / 2.f }
        );
        sprite.setPosition(position);
    }
};

std::vector<Bullet> bullets;
const float bulletSpeed = 1000.f;
int points = 0;

const unsigned int screen_width = 1920;
const unsigned int screen_height = 1080;


GameStateID currentState;
GameStateID previousState;

sf::Texture play_texture;
sf::Texture settings_texture;
sf::Texture leave_texture;
sf::Texture background_texture;
sf::Texture back_texture;
sf::Texture volume_texture;
sf::Texture music_texture;
sf::Texture sfx_texture;
sf::Texture resume_texture;
sf::Texture pause_texture;
sf::Texture leaderboard_texture;

sf::Sprite play_button{ play_texture };
sf::Sprite settings_button{ settings_texture };
sf::Sprite leave_button{ leave_texture };
sf::Sprite background{ background_texture };
sf::Sprite back{ back_texture };
sf::Sprite volume{ volume_texture };
sf::Sprite music_button{ music_texture };
sf::Sprite sfx_button{ sfx_texture };
sf::Sprite resume{ resume_texture };
sf::Sprite pause{ pause_texture };
sf::Sprite leaderboardText{ leaderboard_texture };

sf::Shader darkenShader;
sf::Shader redFilterShader;

sf::Music music;

sf::Font font;
std::vector<ScoreEntry> leaderboardScores;

Slider volumeSlider;

sf::Clock shootClock;
const float shootDelay = 0.15f;
sf::View m_view;

MapGenerator m_map(100, 100);
MapRenderer m_renderer(m_map);
EnemyManager m_enemyManager;
ExitRenderer m_exitRenderer(32.f);
RespawnRenderer m_respawnRenderer(32.f);

sf::Texture playerTexture;
sf::Texture gunTexture;
sf::Texture bulletTexture;
sf::Sprite gun{ gunTexture };
sf::Sprite player{ playerTexture };
sf::CircleShape debugCircle(20.f);

sf::FloatRect getPlayerHitbox(const sf::Sprite& player)
{
    auto box = player.getGlobalBounds();

    box.size *= 0.6f;
    box.position += box.size * 0.2f;    

    return box;
}

bool checkWallCollision(const sf::FloatRect& box, const MapGenerator& map)
{
    int left = static_cast<int>(box.position.x) / 32;
    int right = static_cast<int>(box.position.x + box.size.x) / 32;
    int top = static_cast<int>(box.position.y) / 32;
    int bottom = static_cast<int>(box.position.y + box.size.y) / 32;

    for (int y = top; y <= bottom; y++)
    {
        for (int x = left; x <= right; x++)
        {
            if (x < 0 || y < 0 || x >= map.width || y >= map.height)
                continue;

            if (map.grid[y][x] == WALL)
                return true;
        }
    }
    return false;
}

void respawnPlayer(sf::Sprite& player, const MapGenerator& map, float tileSize)
{
    if (map.respawns.empty())
        return;

    const auto& spawn = map.respawns.front();

    player.setPosition({
        spawn.x * tileSize + tileSize / 2.f,
        spawn.y * tileSize + tileSize / 2.f
        });
}

sf::Vector2f getPlayerInput()
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
        movement += {0.f, -1.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
        movement += {1.f, 0.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
        movement += {0.f, 1.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
        movement += {-1.f, 0.f};

    return movement;
}

void updatePlayer(
    sf::Sprite& player,
    sf::Vector2f movement,
    float dt,
    float speed,
    const MapGenerator& map)
{
    sf::Vector2f delta = movement * speed * dt;

    // X axis
    player.move({ delta.x, 0.f });
    if (checkWallCollision(getPlayerHitbox(player), map))
        player.move({ -delta.x, 0.f });

    // Y axis
    player.move({ 0.f, delta.y });
    if (checkWallCollision(getPlayerHitbox(player), map))
        player.move({ 0.f, -delta.y });
}


void updateGun(sf::Sprite& gun, sf::Sprite& player, sf::RenderWindow& window, float gunDistance)
{
    //calculating the vector of gun postion based on mouse movement
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    sf::Vector2f playerToMouse = worldPos - player.getPosition();
    //for sprite flipping use negative scale values
    //we will calculate the angle of rotation based on our position vector
    gun.setRotation(playerToMouse.angle());
    if (gun.getRotation().asDegrees() > 90.0f && gun.getRotation().asDegrees() < 270.0f) {
        gun.setScale(sf::Vector2f(1.f, -1.f));
        player.setScale(sf::Vector2f(-1.f, 1.f));
    }
    else {
        gun.setScale(sf::Vector2f(1.f, 1.f));
        player.setScale(sf::Vector2f(1.f, 1.f));
    }
    gun.setPosition(player.getPosition() + playerToMouse.normalized() * gunDistance);
}

void shoot(std::vector<Bullet>& bullets,
    const sf::Sprite& gun,
    const sf::Sprite& player,
    const sf::RenderWindow& window,
    const sf::Texture& bulletTexture)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    sf::Vector2f dir = worldPos - player.getPosition();
    dir = dir.normalized();


    bullets.emplace_back(
        bulletTexture,
        gun.getPosition(),
        dir * bulletSpeed
    );
    bullets.back().sprite.rotate(dir.angle());
}

bool bulletHitsWall(const Bullet& bullet, const MapGenerator& map)
{
    sf::Vector2f pos = bullet.sprite.getPosition();

    int tileX = static_cast<int>(pos.x) / 32;
    int tileY = static_cast<int>(pos.y) / 32;

    if (tileX < 0 || tileY < 0 ||
        tileX >= map.width || tileY >= map.height)
        return true; // poza map¹ = kasujemy

    return map.grid[tileY][tileX] == WALL;
}

bool bulletHitsEnemy(const Bullet& bullet, EnemyManager& enemyManager)
{
    sf::FloatRect bulletBounds = bullet.sprite.getGlobalBounds();

    for (Enemy& enemy : enemyManager.getEnemies())
    {
        if (!enemy.isAlive())
            continue;

        sf::FloatRect enemyBounds = enemy.getEnemyBounds();

        if (bulletBounds.findIntersection(enemyBounds))
        {
            points++;
            enemy.kill();
            return true;
        }
    }

    return false;
}

void updateBullets(
    std::vector<Bullet>& bullets,
    float dt,
    const MapGenerator& map)
{
    for (size_t i = 0; i < bullets.size(); )
    {
        bullets[i].sprite.move(bullets[i].velocity * dt);

        if (bulletHitsWall(bullets[i], map))
        {
            bullets.erase(bullets.begin() + i);
        }
        else if (bulletHitsEnemy(bullets[i], m_enemyManager))
        {
            bullets.erase(bullets.begin() + i);
            m_enemyManager.update();
        }
        else
        {
            ++i;
        }
    }
}

void drawBullets(sf::RenderWindow& window, const std::vector<Bullet>& bullets)
{
    for (const auto& b : bullets)
        window.draw(b.sprite);
}

static void render(sf::RenderWindow& window, const sf::Sprite& player, const sf::Sprite& gun)
{

    
    
    debugCircle.setPosition(player.getPosition());
    window.clear();
    //window.draw(debugCircle);
    window.setView(m_view);
    m_renderer.draw(window);
    drawBullets(window, bullets);
    m_enemyManager.draw(window, player);
    m_exitRenderer.draw(window, m_map);
    m_respawnRenderer.draw(window, m_map);
    window.draw(player);
    window.draw(gun);
    window.display();
}

bool compareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score;
}

void saveScoreToFile(std::string& name, int score) {
    std::fstream file("Resources/leaderboard.txt", std::ios::app);

    if (file.is_open()) {
        file << name << " " << score << "\n";
        file.close();
    }
    else {
        std::cerr << "ERROR LOADING LEADERBOARD.TXT\n";
    }
}

std::vector<ScoreEntry> loadLeaderboard() {

    std::vector<ScoreEntry> scores;
    std::ifstream file("Resources/leaderboard.txt");

    if(!file.is_open()) {
        std::cerr << "ERROR LOADING LEADERBOARD.TXT\n";
        return scores;
    }

    std::string name;
    int score;

    while (file >> name >> score) {

        ScoreEntry entry;
        entry.playerName = name;
        entry.score = score;
        scores.push_back(entry);
    }

    file.close();

    std::sort(scores.begin(), scores.end(), compareScores);

    return scores;
}

void initVolumeSlider(float x, float y, float width) {

    volumeSlider.bar.setSize({ width, 6.f });
    volumeSlider.bar.setFillColor(sf::Color::White);
    volumeSlider.bar.setPosition({ x, y });

    volumeSlider.knob.setRadius(10.f);
    volumeSlider.knob.setOrigin({ 10.f, 10.f });
    volumeSlider.knob.setFillColor(sf::Color::Cyan);
    volumeSlider.knob.setPosition({ x + width, y + 3.f });

    volumeSlider.minX = x;
    volumeSlider.maxX = x + width;
    volumeSlider.value = 100.f;
    volumeSlider.dragging = false;
}

void handleSliderEvent(const sf::Event& event, sf::RenderWindow& window) {

    if(event.is<sf::Event::MouseButtonPressed>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if(mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if(volumeSlider.knob.getGlobalBounds().contains(mousePos))
                volumeSlider.dragging = true;
        }
    }

    if(event.is<sf::Event::MouseButtonReleased>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        if (mouse->button == sf::Mouse::Button::Left)
            volumeSlider.dragging = false;
    }

    if(event.is<sf::Event::MouseMoved>() && volumeSlider.dragging) {
        float mouseX = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;

        if (mouseX < volumeSlider.minX) mouseX = volumeSlider.minX;
        if (mouseX > volumeSlider.maxX) mouseX = volumeSlider.maxX;

        volumeSlider.knob.setPosition({ mouseX, volumeSlider.knob.getPosition().y });

        float t = (mouseX - volumeSlider.minX) / (volumeSlider.maxX - volumeSlider.minX);
        volumeSlider.value = t * 100.f;
    }
}

void updateVolumeSlider(sf::Music& music) {
    music.setVolume(volumeSlider.value);
}

void centerOrigin(sf::Sprite &sprite, sf::Texture &texture) {
    sprite.setOrigin({ texture.getSize().x / 2.f, texture.getSize().y / 2.f });
}

void menuHandleEvent(const sf::Event& event, sf::RenderWindow& window) {

    if (event.is<sf::Event::Closed>())
        window.close();

    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (play_button.getGlobalBounds().contains(mousePos))
                //window.clear();
                currentState = GameStateID::GAMEPLAY;

            if (settings_button.getGlobalBounds().contains(mousePos)) {
                previousState = currentState;
                currentState = GameStateID::SETTINGS;
            }

            if (leave_button.getGlobalBounds().contains(mousePos))
                window.close();
        }
    }

    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key->scancode == sf::Keyboard::Scan::Escape)
            window.close();
    }
}

void drawVolumeSlider(sf::RenderWindow& window) {
    window.draw(volumeSlider.bar);
    window.draw(volumeSlider.knob);
}

void menuUpdate(sf::Time, sf::RenderWindow& window) {

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (play_button.getGlobalBounds().contains(mousePos))
        play_button.setScale({ 1.1f,1.1f });
    else play_button.setScale({ 1.f,1.f });

    if (settings_button.getGlobalBounds().contains(mousePos))
        settings_button.setScale({ 1.1f,1.1f });
    else
        settings_button.setScale({ 1.f,1.f });

    if (leave_button.getGlobalBounds().contains(mousePos))
        leave_button.setScale({ 1.1f,1.1f });
    else
        leave_button.setScale({ 1.f,1.f });
}

void menuDraw(sf::RenderWindow& window) {

    window.clear();
    window.draw(background);
    window.draw(play_button);
    window.draw(settings_button);
    window.draw(leave_button);
    window.display();
}

void gameHandleEvent(const sf::Event& event, sf::RenderWindow& window) {

    if(event.is<sf::Event::Closed>())
        window.close();

    if(event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if(key->scancode == sf::Keyboard::Scan::Escape) {
            music.pause();
            currentState = GameStateID::PAUSE_MENU;
        }

        if (key->scancode == sf::Keyboard::Scan::Backspace) {
            leaderboardScores = loadLeaderboard();
            currentState = GameStateID::END_GAME;
        }
    }
}

void gameUpdate(sf::Time time, sf::RenderWindow& window) {
    const float speedMultiplier = 300.f;
    const float gunDistance = 15.0f;
    sf::Vector2f movement = getPlayerInput();
    float dt = time.asSeconds();
    updatePlayer(player, movement, dt, speedMultiplier, m_map);
    m_view.setCenter(player.getPosition());
    updateGun(gun, player, window, gunDistance);
    updateBullets(bullets, dt, m_map);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
        shootClock.getElapsedTime().asSeconds() > shootDelay)
    {
        shoot(bullets, gun, player, window, bulletTexture);
        shootClock.restart();
    }
}

void gameDraw(sf::RenderWindow& window) {
    
    render(window, player, gun);    
}

void settingsHandleEvent(const sf::Event& event, sf::RenderWindow& window) {

    if(event.is<sf::Event::Closed>())
        window.close();

    if(event.is<sf::Event::MouseButtonPressed>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if(mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if(mouse->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if(back.getGlobalBounds().contains(mousePos))
                    currentState = previousState;
            }
        }
    }

    if(event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if(key->scancode == sf::Keyboard::Scan::Escape)
            currentState = previousState;
    }

    handleSliderEvent(event, window);
}

void settingsUpdate(sf::Time, sf::RenderWindow& window) {

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if(back.getGlobalBounds().contains(mousePos))
        back.setScale({ 1.1f,1.1f });
    else
        back.setScale({ 1.f,1.f });

    updateVolumeSlider(music);
}

void settingsDraw(sf::RenderWindow& window) {

    window.clear();
    window.draw(background, &darkenShader);
    window.draw(volume);
    window.draw(music_button);
    window.draw(sfx_button);
    window.draw(back);
    drawVolumeSlider(window);
    window.display();
}

void pauseMenuHandleEvent(const sf::Event& event, sf::RenderWindow& window) {

    if(event.is<sf::Event::Closed>())
        window.close();

    if(event.is<sf::Event::MouseButtonPressed>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if(mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if(resume.getGlobalBounds().contains(mousePos)) {
                music.play();
                currentState = GameStateID::GAMEPLAY;
            }

            if(settings_button.getGlobalBounds().contains(mousePos)) {
                previousState = currentState;
                currentState = GameStateID::SETTINGS;
            }

            if(leave_button.getGlobalBounds().contains(mousePos)) {
                music.play();
                currentState = GameStateID::MENU;
            }
        }
    }

    if(event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key->scancode == sf::Keyboard::Scan::Escape) {
            music.play();
            currentState = GameStateID::GAMEPLAY;
        }
    }

}

void pauseMenuUpdate(sf::Time, sf::RenderWindow& window) {

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if(resume.getGlobalBounds().contains(mousePos))
        resume.setScale({ 1.1f,1.1f });
    else resume.setScale({ 1.f,1.f });

    if(settings_button.getGlobalBounds().contains(mousePos))
        settings_button.setScale({ 1.1f,1.1f });
    else
        settings_button.setScale({ 1.f,1.f });

    if(leave_button.getGlobalBounds().contains(mousePos))
        leave_button.setScale({ 1.1f,1.1f });
    else
        leave_button.setScale({ 1.f,1.f });

}

void pauseMenuDraw(sf::RenderWindow& window) {

    window.clear();
    window.draw(background, &redFilterShader);
    window.draw(resume);
    window.draw(settings_button);
    window.draw(leave_button);
    window.draw(pause);
    window.display();
}

void endMenuHandleEvent(const sf::Event& event, sf::RenderWindow& window) {

    if (event.is<sf::Event::Closed>())
        window.close();

    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key->scancode == sf::Keyboard::Scan::Escape || key->scancode == sf::Keyboard::Scan::Enter) {
            currentState = GameStateID::MENU;
        }
    }
}

void endMenuUpdate(sf::Time, sf::RenderWindow& window) {

    //sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // end menu
}

void endMenuDraw(sf::RenderWindow& window) {


    window.clear();
    window.draw(background, &darkenShader);
    window.draw(leaderboardText);

    static sf::Text entryText(font, "", 40);
    entryText.setFillColor(sf::Color::White);

    float startX = screen_width / 2.f - 300.f;
    float startY = screen_height / 2.f - 200.f;

    for (size_t i = 0; i < leaderboardScores.size() && i < 10; ++i) {

        std::string text =
            std::to_string(i + 1) + ". " +
            leaderboardScores[i].playerName + " ..... " + std::to_string(leaderboardScores[i].score);

        entryText.setString(text);
        entryText.setPosition({ startX, startY + i * 50.f });

        window.draw(entryText);
    }

    window.display();
}



GameState states[] = {
    { menuHandleEvent, menuUpdate, menuDraw },
    { gameHandleEvent, gameUpdate, gameDraw },
    { settingsHandleEvent, settingsUpdate, settingsDraw },
    { endMenuHandleEvent, endMenuUpdate, endMenuDraw },
    { pauseMenuHandleEvent, pauseMenuUpdate, pauseMenuDraw },
};


int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); //wa¿ne ¿eby losowe genrowanie dzia³a³o

    if (!play_texture.loadFromFile("Resources/Images/Sprites/Graj.png") ||
        !settings_texture.loadFromFile("Resources/Images/Sprites/Ustawienia.png") ||
        !leave_texture.loadFromFile("Resources/Images/Sprites/Wyjdz.png") ||
        !background_texture.loadFromFile("Resources/Images/Backgrounds/tlo.jpg") ||
        !back_texture.loadFromFile("Resources/Images/Sprites/wroc.png") ||
        !volume_texture.loadFromFile("Resources/Images/Sprites/glosnosc.png") ||
        !music_texture.loadFromFile("Resources/Images/Sprites/muzyka.png") ||
        !sfx_texture.loadFromFile("Resources/Images/Sprites/sfx.png") ||
        !resume_texture.loadFromFile("Resources/Images/Sprites/wznow.png") ||
        !pause_texture.loadFromFile("Resources/Images/Sprites/pauza.png") ||
        !leaderboard_texture.loadFromFile("Resources/Images/Sprites/leaderboard.png") ||
        !music.openFromFile("Resources/Sounds/music.wav") ||
        !font.openFromFile("Resources/Fonts/PixelifySans.ttf") ||
        !darkenShader.loadFromFile("Resources/Shaders/darken.frag", sf::Shader::Type::Fragment) ||
        !redFilterShader.loadFromFile("Resources/Shaders/redFilter.frag", sf::Shader::Type::Fragment)) {

        std::cerr << "ERROR LOADING FILES\n";
        return -1;
    }

    initVolumeSlider(screen_width / 2.f + music_texture.getSize().x * 0.5f, screen_height / 2.f - music_texture.getSize().y, 400.f);

    play_button = sf::Sprite(play_texture);
    settings_button = sf::Sprite(settings_texture);
    leave_button = sf::Sprite(leave_texture);
    background = sf::Sprite(background_texture);
    volume = sf::Sprite(volume_texture);
    back = sf::Sprite(back_texture);
    music_button = sf::Sprite(music_texture);
    sfx_button = sf::Sprite(sfx_texture);
    resume = sf::Sprite(resume_texture);
    pause = sf::Sprite(pause_texture);
    leaderboardText = sf::Sprite(leaderboard_texture);

    //ustawienie srodkow
    centerOrigin(play_button, play_texture);
    centerOrigin(settings_button, settings_texture);
    centerOrigin(leave_button, leave_texture);
    centerOrigin(volume, volume_texture);
    centerOrigin(music_button, music_texture);
    centerOrigin(sfx_button, sfx_texture);
    centerOrigin(back, back_texture);
    centerOrigin(resume, resume_texture);
    centerOrigin(pause, pause_texture);
    centerOrigin(leaderboardText, leaderboard_texture);

    sf::RenderWindow window(sf::VideoMode({ screen_width, screen_height }), "Simple call!");

    window.setFramerateLimit(60);
    int move_buttons = 2 * settings_texture.getSize().y;

    play_button.setPosition({ screen_width / 2.f, screen_height / 2.f - move_buttons });
    settings_button.setPosition({ screen_width / 2.f, screen_height / 2.f });
    leave_button.setPosition({ screen_width / 2.f, screen_height / 2.f + move_buttons });

    volume.setPosition({ screen_width / 2.f, volume_texture.getSize().y * 2.f });
    music_button.setPosition({ screen_width / 2.f - music_texture.getSize().x * 1.1f, screen_height / 2.f - music_texture.getSize().y });
    sfx_button.setPosition({ screen_width / 2.f - music_texture.getSize().x * 1.1f, screen_height / 2.f + sfx_texture.getSize().y });
    back.setPosition({ screen_width / 2.f, screen_height - back_texture.getSize().y * 1.6f });
    
    if (!playerTexture.loadFromFile("images/test-player.png")) {
        std::cerr << "Failed to load image\n";
    }    
    if (!gunTexture.loadFromFile("images/gun.png")) {
        std::cerr << "Failed to load image\n";
    }    

    if (!bulletTexture.loadFromFile("images/bullet.png")) {
        std::cerr << "Failed to load image\n";
    }
    
    /*sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("images/test-enemy.png")) {
        std::cerr << "Failed to load image\n";
    }*/
    //Enemy enemy(enemyTexture, { 960.f, 540.f });

    sf::Clock clock;    

    float totalWidth = 100 * 32.0f;
    float totalHeight = 100 * 32.0f;

    m_view.setSize({ 1920, 1080 });
    m_view.zoom(.2f);

    m_map.reset();
    m_enemyManager.spawnFromMap(m_map, 32.f);
    player = sf::Sprite(playerTexture);
    gun = sf::Sprite(gunTexture);
    player.setOrigin(sf::Vector2f(playerTexture.getSize().x / 2.f, playerTexture.getSize().y / 2.f));
    gun.setOrigin({24, 9});
    //gun.setOrigin(sf::Vector2f(gunTexture.getSize().x / 2.f, gunTexture.getSize().y / 2.f));
    respawnPlayer(player, m_map, 32.f);

    debugCircle.setFillColor(sf::Color::Red);
    debugCircle.setOrigin({ 20.f, 20.f });

    resume.setPosition(play_button.getPosition());
    pause.setPosition(volume.getPosition());

    leaderboardText.setPosition(pause.getPosition());    
    //petla programu
    currentState = GameStateID::MENU;    

    music.setLooping(true);
    music.play();

    while (window.isOpen())
    {
        sf::Time dtTime = clock.restart();
        float dt = dtTime.asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            states[(int)currentState].handleEvent(*event, window);
            if (event->is<sf::Event::Closed>())
                window.close();

            //if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {

            //    if (k->code == sf::Keyboard::Key::Space) {
            //        m_map.reset();
            //        /*m_enemyManager.spawnFromMap(m_map, TILE_SIZE);*/

            //        /*for (auto& s : m_map.enemySpawns) {
            //            m_enemies.push_back({ s.x, s.y });
            //        }*/
            //    }

            //}
        }
        states[(int)currentState].update(dtTime, window);
        states[(int)currentState].draw(window);                                   
    }
}