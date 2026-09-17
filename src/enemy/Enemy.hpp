#pragma once
#include <SFML/Graphics.hpp>
#include "../map/Map.hpp"

class Enemy {
public:
    Enemy(int gridX, int gridY, float tileSize, const sf::Texture& texture, const sf::Texture& knife_texture);

    void updateKnife(const sf::Sprite& player);
    int updateEnemy(const sf::Sprite& player, float dt, const MapGenerator& map);
    void draw(const sf::Sprite& player, sf::RenderWindow& window);
    sf::FloatRect getEnemyBounds() const;
    void kill() {
        if (!alive) return;  
        alive = false;     
    };
    bool isAlive() const { return alive; }
    void setSpeed(int n_speed) {
        speed = n_speed;
    };

private:    
    sf::Sprite e_sprite;
    sf::Sprite e_knife;
    bool alive = true;
    float knifeDistance = 15.0f;
    float speed = 180.f; //players is 300
};
