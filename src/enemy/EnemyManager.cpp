#include "EnemyManager.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
void EnemyManager::spawnFromMap(const MapGenerator& map, float tileSize) {
    m_enemies.clear();
    
    if (!enemyTexture.loadFromFile("images/test-enemy.png")) {
        std::cerr << "Failed to load image\n";
    }
    
    if (!knifeTexture.loadFromFile("images/knife.png")) {
        std::cerr << "Failed to load image\n";
    }

    for (const auto& s : map.enemySpawns) {
        m_enemies.emplace_back(s.x, s.y, tileSize, enemyTexture, knifeTexture);
        //std::cout << "enemy drew";
    }
}

void EnemyManager::clear() {
    m_enemies.clear();
}
const std::vector<Enemy> EnemyManager::getEnemies() const{
    return m_enemies;
}
void EnemyManager::draw(sf::RenderWindow& window, const sf::Sprite& player) {
    for (auto& e : m_enemies) {
        e.draw(player, window);        
    }
}

std::vector<Enemy>& EnemyManager::getEnemies()
{
    return m_enemies;
}


void EnemyManager::update(float dt, const MapGenerator& map, const sf::Sprite& player)
{

    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const Enemy& e)
            {
                return !e.isAlive();
            }),
        m_enemies.end()
    );    
    for (auto& e : m_enemies) {
        e.updateEnemy(player, dt, map);        
    }
}

