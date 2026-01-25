#include "RespawnRenderer.hpp"

RespawnRenderer::RespawnRenderer(float tileSize)
    : m_tileSize(tileSize)
{
    m_spawnShape.setSize({ m_tileSize, m_tileSize });
    m_spawnShape.setFillColor(sf::Color::Blue); // niebieski dla gracza
}

void RespawnRenderer::draw(sf::RenderWindow& window, const MapGenerator& map) {
    for (auto& r : map.respawns) {
        m_spawnShape.setPosition(sf::Vector2f(r.x * m_tileSize, r.y * m_tileSize));
        window.draw(m_spawnShape);
    }
}
