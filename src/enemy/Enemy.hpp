#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(int gridX, int gridY, float tileSize);

    void draw(sf::RenderWindow& window) const;

private:
    sf::RectangleShape m_shape;
};
