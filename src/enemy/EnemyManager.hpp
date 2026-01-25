#pragma once
#include <vector>
#include "Enemy.hpp"
#include "../map/Map.hpp"

class EnemyManager {
public:
    void spawnFromMap(const MapGenerator& map, float tileSize);
    void clear();

    void draw(sf::RenderWindow& window) const;

private:
    std::vector<Enemy> m_enemies;
};
