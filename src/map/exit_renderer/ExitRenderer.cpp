#include "ExitRenderer.hpp"

ExitRenderer::ExitRenderer(float tileSize)
    : m_tileSize(tileSize)
{
    m_exitShape.setSize({ m_tileSize, m_tileSize });
    m_exitShape.setFillColor(sf::Color::Green);
}

void ExitRenderer::draw(sf::RenderWindow& window, const MapGenerator& map) {
    for (auto& e : map.exits) {
        m_exitShape.setPosition(sf::Vector2f(e.x * m_tileSize, e.y * m_tileSize));
        window.draw(m_exitShape);
    }
}
   
