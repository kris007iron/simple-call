#pragma once
#include <SFML/Graphics.hpp>
#include "../Map.hpp" 

class RespawnRenderer {
public:
    RespawnRenderer(float tileSize);

    void draw(sf::RenderWindow& window, const MapGenerator& map);

private:
    float m_tileSize;
    sf::RectangleShape m_spawnShape;
};
