#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>

enum class GameStateID {
    MENU,
    GAMEPLAY,
    SETTINGS,
    END_GAME,
    PAUSE_MENU,
};

struct GameState {
    void (*handleEvent)(const sf::Event&);
    void (*update)(sf::Time);
    void (*draw)();
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

const unsigned int screen_width = 1920;
const unsigned int screen_height = 1080;

sf::RenderWindow window;

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

void handleSliderEvent(const sf::Event& event) {

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

void menuHandleEvent(const sf::Event& event) {

    if(event.is<sf::Event::Closed>())
        window.close();

    if(event.is<sf::Event::MouseButtonPressed>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if(mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if(play_button.getGlobalBounds().contains(mousePos))
                currentState = GameStateID::GAMEPLAY;

            if(settings_button.getGlobalBounds().contains(mousePos)) {
                previousState = currentState;
                currentState = GameStateID::SETTINGS;
            }

            if(leave_button.getGlobalBounds().contains(mousePos))
                window.close();
        }
    }

    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key->scancode == sf::Keyboard::Scan::Escape)
            window.close();
    }
}

void drawVolumeSlider() {
    window.draw(volumeSlider.bar);
    window.draw(volumeSlider.knob);
}

void menuUpdate(sf::Time) {

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

void menuDraw() {

    window.clear();
    window.draw(background);
    window.draw(play_button);
    window.draw(settings_button);
    window.draw(leave_button);
    window.display();
}

void gameHandleEvent(const sf::Event& event) {

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

void gameUpdate(sf::Time) {}

void gameDraw() {

    window.clear(sf::Color::Black);
    window.display();
}

void settingsHandleEvent(const sf::Event& event) {

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

    handleSliderEvent(event);
}

void settingsUpdate(sf::Time) {

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if(back.getGlobalBounds().contains(mousePos))
        back.setScale({ 1.1f,1.1f });
    else
        back.setScale({ 1.f,1.f });

    updateVolumeSlider(music);
}

void settingsDraw() {

    window.clear();
    window.draw(background, &darkenShader);
    window.draw(volume);
    window.draw(music_button);
    window.draw(sfx_button);
    window.draw(back);
    drawVolumeSlider();
    window.display();
}

void pauseMenuHandleEvent(const sf::Event& event) {

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

        if(key->scancode == sf::Keyboard::Scan::Escape)
            music.play();
            currentState = GameStateID::GAMEPLAY;
    }

}

void pauseMenuUpdate(sf::Time) {

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

void pauseMenuDraw() {

    window.clear();
    window.draw(background, &redFilterShader);
    window.draw(resume);
    window.draw(settings_button);
    window.draw(leave_button);
    window.draw(pause);
    window.display();
}

void endMenuHandleEvent(const sf::Event& event) {

    if (event.is<sf::Event::Closed>())
        window.close();

    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key->scancode == sf::Keyboard::Scan::Escape || key->scancode == sf::Keyboard::Scan::Enter) {
            currentState = GameStateID::MENU;
        }
    }
}

void endMenuUpdate(sf::Time) {

    //sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // end menu
}

void endMenuDraw() {


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


int main() {

    window = sf::RenderWindow(sf::VideoMode({ screen_width, screen_height }), "Gierka");
    window.setFramerateLimit(60);

    if(!play_texture.loadFromFile("Resources/Images/Sprites/Graj.png") ||
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


    //pozycje
    int move_buttons = 2 * settings_texture.getSize().y;

    play_button.setPosition({ screen_width / 2.f, screen_height / 2.f - move_buttons });
    settings_button.setPosition({ screen_width / 2.f, screen_height / 2.f });
    leave_button.setPosition({ screen_width / 2.f, screen_height / 2.f + move_buttons });

    volume.setPosition({ screen_width / 2.f, volume_texture.getSize().y * 2.f });
    music_button.setPosition({ screen_width / 2.f - music_texture.getSize().x * 1.1f, screen_height / 2.f - music_texture.getSize().y });
    sfx_button.setPosition({ screen_width / 2.f - music_texture.getSize().x * 1.1f, screen_height / 2.f + sfx_texture.getSize().y });
    back.setPosition({ screen_width / 2.f, screen_height - back_texture.getSize().y * 1.6f });

    resume.setPosition(play_button.getPosition());
    pause.setPosition(volume.getPosition());

    leaderboardText.setPosition(pause.getPosition());

    //petla programu
    currentState = GameStateID::MENU;
    sf::Clock clock;

    music.setLooping(true);
    music.play();

    while(window.isOpen()) {
        sf::Time dt = clock.restart();

        while(auto event = window.pollEvent())
            states[(int)currentState].handleEvent(*event);

        states[(int)currentState].update(dt);
        states[(int)currentState].draw();

    }

    return 0;
}
