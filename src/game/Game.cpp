#include "game.hpp"
#include <optional>

// Konstruktor klasy Game
Game::Game()
    : m_window(sf::VideoMode({ (unsigned int)(MAP_W * 32), (unsigned int)(MAP_H * 32) }), "Dungeon Crawler"),
    m_map(MAP_W, MAP_H),
    m_renderer(m_map)
{
    m_window.setFramerateLimit(60);
}

void Game::run() {
    while (m_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::update() {
 
}


// Obs³uga klawiatury i okna
void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {
            // Reset mapy pod spacj¹
            if (k->code == sf::Keyboard::Key::Space) {
                m_map.reset();

            }
        }
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    m_renderer.draw(m_window);

    m_window.display();
}