#pragma once
#include <vector>
#include "../map/Map.hpp"
#include "Enemy.hpp"

class EnemyManager {
public:
    void spawnFromMap(const MapGenerator& map, float tileSize);
    void clear();

    void draw(sf::RenderWindow& window) const;

private:
    std::vector<Enemy> m_enemies;
    sf::Texture enemyTexture;
};
