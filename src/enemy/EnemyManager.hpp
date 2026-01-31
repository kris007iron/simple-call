#pragma once
#include <vector>
#include "../map/Map.hpp"
#include "Enemy.hpp"

class EnemyManager {
public:
    void spawnFromMap(const MapGenerator& map, float tileSize, int lvl);
    void clear();    
    void draw(sf::RenderWindow& window, const sf::Sprite& player);
    std::vector<Enemy>& getEnemies();
    const std::vector<Enemy> getEnemies() const;
    void update(float dt, const MapGenerator& map, const sf::Sprite& player);
    void setHealth(int n_health) {
        health = n_health;
    };
    int getHealth() const {
        return health;
    };
private:
    std::vector<Enemy> m_enemies;
    sf::Texture enemyTexture;
    sf::Texture knifeTexture;
    int health = 20;
};
