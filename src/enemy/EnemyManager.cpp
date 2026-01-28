#include "EnemyManager.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
void EnemyManager::spawnFromMap(const MapGenerator& map, float tileSize) {
    m_enemies.clear();
    
    if (!enemyTexture.loadFromFile("images/test-enemy.png")) {
        std::cerr << "Failed to load image\n";
    }

    for (const auto& s : map.enemySpawns) {
        m_enemies.emplace_back(s.x, s.y, tileSize, enemyTexture);
        //std::cout << "enemy drew";
    }
}

void EnemyManager::clear() {
    m_enemies.clear();
}

void EnemyManager::draw(sf::RenderWindow& window) const {
    for (const auto& e : m_enemies) {
        e.draw(window);
        //std::cout << "enemy drew";
    }
}
