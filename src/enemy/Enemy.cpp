#include "Enemy.hpp"

Enemy::Enemy(int gridX, int gridY, float tileSize) {
    m_shape.setSize({ tileSize, tileSize });
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition({
        gridX * tileSize,
        gridY * tileSize
        });
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

//bool initEnemy(Enemy& enemy, sf::Texture& texture, sf::Vector2f startPos)
//{
//    enemy.sprite.setTexture(texture);
//    enemy.sprite.setPosition(startPos);
//    enemy.speed = 100.f;
//    enemy.alive = true;
//    return true;
//}
//
//void updateEnemy(Enemy& enemy, float deltaTime)
//{
//    if (!enemy.alive)
//        return;
//
//    enemy.sprite.move({ -enemy.speed * deltaTime, 0.f });
//}
//
////update 
//void drawEnemy(const Enemy& enemy, sf::RenderWindow& window)
//{
//    if (enemy.alive)
//        window.draw(enemy.sprite);
//}
//
//
//sf::FloatRect getEnemyBounds(const Enemy& enemy)
//{
//    return enemy.sprite.getGlobalBounds();
//}
