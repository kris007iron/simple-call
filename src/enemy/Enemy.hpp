#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(int gridX, int gridY, float tileSize,const sf::Texture& texture);

    void draw(sf::RenderWindow& window) const;

private:    
    sf::Sprite e_sprite;
};
