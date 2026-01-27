#include "EnemyManager.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
void EnemyManager::spawnFromMap(const MapGenerator& map, float tileSize) {
    m_enemies.clear();

    for (const auto& s : map.enemySpawns) {
        //m_enemies.emplace_back(s.x, s.y, tileSize);
        std::cout << "enemy drew";
    }
}

void EnemyManager::clear() {
    m_enemies.clear();
}

void EnemyManager::draw(sf::RenderWindow& window) const {
    for (const auto& e : m_enemies) {
        //e.draw(window);
        std::cout << "enemy drew";
    }
}
