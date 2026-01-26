#include <SFML/Graphics.hpp>
#include <iostream>
#include "enemy/Enemy.h"

struct Bullet
{
    sf::Sprite sprite;
    sf::Vector2f velocity;

    Bullet(const sf::Texture& tex,
        sf::Vector2f position,
        sf::Vector2f vel)
        : sprite(tex), velocity(vel)
    {
        sprite.setOrigin(
            { tex.getSize().x / 2.f,
            tex.getSize().y / 2.f }
        );
        sprite.setPosition(position);
    }
};

struct EnemyBullet{};





std::vector<Bullet> bullets;
const float bulletSpeed = 1000.f;

std::vector<EnemyBullet> enemyBullets;

std::vector<Enemy> enemys;

sf::Clock shootClock;
const float shootDelay = 0.15f;


sf::Vector2f getPlayerInput()
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
        movement += {0.f, -1.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
        movement += {1.f, 0.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
        movement += {0.f, 1.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
        movement += {-1.f, 0.f};

    return movement;
}

void updatePlayer(sf::Sprite& player, sf::Vector2f movement, float dt, float speed)
{
    player.move(movement * dt * speed);
}

void updateGun(sf::Sprite& gun, sf::Sprite& player, sf::RenderWindow& window, float gunDistance)
{
    //calculating the vector of gun postion based on mouse movement
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
    sf::Vector2f playerToMouse = mousePos - player.getPosition();
    //for sprite flipping use negative scale values
    //we will calculate the angle of rotation based on our position vector
    gun.setRotation(playerToMouse.angle());
    if (gun.getRotation().asDegrees() > 90.0f && gun.getRotation().asDegrees() < 270.0f) {
        gun.setScale(sf::Vector2f(15.f, -15.f));
        player.setScale(sf::Vector2f(-20.f, 20.f));
    }
    else{
        gun.setScale(sf::Vector2f(15.f, 15.f));
        player.setScale(sf::Vector2f(20.f, 20.f));
    }
    gun.setPosition(player.getPosition() + playerToMouse.normalized() * gunDistance);
}

void shoot(std::vector<Bullet>& bullets,
           const sf::Sprite& gun,
           const sf::Sprite& player,
           const sf::RenderWindow& window,
           const sf::Texture& bulletTexture)
{
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
    sf::Vector2f dir = mousePos - player.getPosition();
    dir = dir.normalized();
    

    bullets.emplace_back(
        bulletTexture,
        gun.getPosition(),
        dir * bulletSpeed
    );
    bullets.back().sprite.rotate(dir.angle());
}

void updateBullets(std::vector<Bullet>& bullets,
    float dt,
    const sf::RenderWindow& window)
{
    const sf::Vector2u size = window.getSize();

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [&](const Bullet& b)
            {
                sf::Vector2f p = b.sprite.getPosition();
                return p.x < 0 || p.x > size.x ||
                    p.y < 0 || p.y > size.y;
            }),
        bullets.end()
    );

    for (auto& b : bullets)
        b.sprite.move(b.velocity * dt);
}


void drawBullets(sf::RenderWindow& window, const std::vector<Bullet>& bullets)
{
    for (const auto& b : bullets)
        window.draw(b.sprite);
}


static void render(sf::RenderWindow& window, const sf::Sprite& player, const sf::Sprite& gun)
{
    window.clear();
    window.draw(player);
    window.draw(gun);
    drawBullets(window, bullets);    
    window.display();
}


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Simple call!");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("images/test-player.png")) {
        std::cerr << "Failed to load image\n";
    }
    sf::Sprite player(playerTexture);
    player.setOrigin(sf::Vector2f(playerTexture.getSize().x / 2.f, playerTexture.getSize().y / 2.f));
    player.setScale(sf::Vector2f(20.f, 20.f));
    player.setPosition(sf::Vector2f(960.f, 540.f));

    sf::Texture gunTexture;
    if (!gunTexture.loadFromFile("images/gun.png")) {
        std::cerr << "Failed to load image\n";        
    }
    sf::Sprite gun(gunTexture);
    gun.setOrigin(sf::Vector2f(gunTexture.getSize().x / 2.f, gunTexture.getSize().y / 2.f));
    gun.setScale(sf::Vector2f(15.f, 15.f));

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("images/bullet.png")) {
        std::cerr << "Failed to load image\n";
    }

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("images/test-enemy.png")) {
        std::cerr << "Failed to load image\n";
    }
    Enemy enemy(enemyTexture, { 960.f, 540.f });
    initEnemy(enemy, enemyTexture, { 960.f, 540.f });

    sf::Clock clock;
    const float speedMultiplier = 300.f;
    const float gunDistance = 200.f;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Vector2f movement = getPlayerInput();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
            shootClock.getElapsedTime().asSeconds() > shootDelay)
        {
            shoot(bullets, gun, player, window, bulletTexture);
            shootClock.restart();
        }
        updateEnemy(enemy, dt);

        updatePlayer(player, movement, dt, speedMultiplier);
        updateGun(gun, player, window, gunDistance);
        updateBullets(bullets, dt, window);
        drawEnemy(enemy, window);
        render(window, player, gun);
    }
}
