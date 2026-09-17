#pragma once
#include <SFML/Graphics.hpp>
#include "../Map.hpp"

class ExitRenderer {
public:
    ExitRenderer(float tileSize);

    void draw(sf::RenderWindow& window, const MapGenerator& map);

private:
    float m_tileSize;
    sf::RectangleShape m_exitShape;
};
