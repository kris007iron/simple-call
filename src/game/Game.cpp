#include "game.hpp"
#include <optional>

Game::Game()
    : m_window(sf::VideoMode({ 1280, 720 }), "Dungeon Crawler"),
    m_map(MAP_W, MAP_H),
    m_renderer(m_map),
    m_enemyManager(),
    m_exitRenderer(TILE_SIZE),
    m_respawnRenderer(TILE_SIZE) // <- inicjalizacja respawn
{
    m_window.setFramerateLimit(60);

    float totalWidth = MAP_W * TILE_SIZE;
    float totalHeight = MAP_H * TILE_SIZE;

    m_view.setSize({ totalWidth, totalHeight });
    m_view.setCenter({ totalWidth / 2.0f, totalHeight / 2.0f });

    m_map.reset();
    m_enemyManager.spawnFromMap(m_map, TILE_SIZE);
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

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {
         
            if (k->code == sf::Keyboard::Key::Space) {
                m_map.reset();
                m_enemyManager.spawnFromMap(m_map, TILE_SIZE);

                for (auto& s : m_map.enemySpawns) {
                    m_enemies.push_back({ s.x, s.y });
                }
            }

        }
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);
    m_window.setView(m_view);

    m_renderer.draw(m_window);
    m_enemyManager.draw(m_window);
    m_exitRenderer.draw(m_window, m_map);
    m_respawnRenderer.draw(m_window, m_map);

    m_window.display();
}





