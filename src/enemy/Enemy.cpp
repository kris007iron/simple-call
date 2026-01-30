#include "Enemy.hpp"
#include <iostream>
#include "../map/Map.hpp"

Enemy::Enemy(int gridX, int gridY, float tileSize, const sf::Texture& texture, const sf::Texture& knife_texture)
    : e_sprite(texture), e_knife(knife_texture)
{
    e_sprite.setPosition({
        gridX * tileSize,
        gridY * tileSize
        });
    e_knife.setPosition({
        gridX * tileSize,
        gridY * tileSize
        });

}

void Enemy::draw(const sf::Sprite& player, sf::RenderWindow& window) {    
    updateKnife(player);
    window.draw(e_sprite);
    window.draw(e_knife);
}

void Enemy::updateEnemy(const sf::Sprite& player, float dt, const MapGenerator& map)
{
    sf::Vector2f toPlayer = player.getPosition() - e_sprite.getPosition();
    float distance = toPlayer.length();

    if (distance < 40.f)
    {
        // ATTACK RANGE        
        // here you would trigger attack logic
    }
    else if (distance < 100.f)
    {
        // CHASE RANGE
        std::cout << "pursue\n";

        sf::Vector2f dir = toPlayer.normalized();
        

        e_sprite.move(dir * speed * dt);
    }
}


void Enemy::updateKnife(const sf::Sprite & player)
{
    //calculating the vector of gun postion based on mouse movement        
    sf::Vector2f worldPos = player.getPosition();
    sf::Vector2f playerToMouse = worldPos - e_sprite.getPosition();
    //for sprite flipping use negative scale values
    //we will calculate the angle of rotation based on our position vector
    e_knife.setRotation(playerToMouse.angle());
    if (e_knife.getRotation().asDegrees() > 90.0f && e_knife.getRotation().asDegrees() < 270.0f) {
        e_knife.setScale(sf::Vector2f(1.f, -1.f));
        //e_sprite.setScale(sf::Vector2f(-1.f, 1.f));
    }
    else {
        e_knife.setScale(sf::Vector2f(1.f, 1.f));
        //e_sprite.setScale(sf::Vector2f(1.f, 1.f));
    }
    e_knife.setPosition(e_sprite.getPosition() + playerToMouse.normalized() * knifeDistance);
}

sf::FloatRect Enemy::getEnemyBounds() const
{
    return e_sprite.getGlobalBounds();
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

