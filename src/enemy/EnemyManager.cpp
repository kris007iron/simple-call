#include "EnemyManager.hpp"

void EnemyManager::spawnFromMap(const MapGenerator& map, float tileSize) {
    m_enemies.clear();

    for (const auto& s : map.enemySpawns) {
        m_enemies.emplace_back(s.x, s.y, tileSize);
    }
}

void EnemyManager::clear() {
    m_enemies.clear();
}

void EnemyManager::draw(sf::RenderWindow& window) const {
    for (const auto& e : m_enemies) {
        e.draw(window);
    }
}
