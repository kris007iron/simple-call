#include "Enemy.hpp"

Enemy::Enemy(int gridX, int gridY, float tileSize) {
    m_shape.setSize({ tileSize, tileSize });
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition({
        gridX * tileSize,
        gridY * tileSize
        });
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}
