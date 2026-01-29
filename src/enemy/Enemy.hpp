#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(int gridX, int gridY, float tileSize, const sf::Texture& texture, const sf::Texture& knife_texture);

    void updateKnife(const sf::Sprite& player);
    void draw(const sf::Sprite& player, sf::RenderWindow& window);
    sf::FloatRect getEnemyBounds() const;
    void kill() {
        if (!alive) return;  
        alive = false;     
    };
    bool isAlive() const { return alive; }


private:    
    sf::Sprite e_sprite;
    sf::Sprite e_knife;
    bool alive = true;
    float knifeDistance = 15.0f;
};
