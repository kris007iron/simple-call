#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include "map/Map.hpp"
#include "map/map_renderer/MapRenderer.hpp"
#include "enemy/EnemyManager.hpp"
#include "map/exit_renderer/ExitRenderer.hpp"
#include "map/respawn_renderer/RespawnRenderer.hpp"

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

//std::vector<Enemy> enemys;

sf::Clock shootClock;
const float shootDelay = 0.15f;
sf::View m_view;

MapGenerator m_map(100,100);
MapRenderer m_renderer(m_map);
EnemyManager m_enemyManager;
ExitRenderer m_exitRenderer(32.f);
RespawnRenderer m_respawnRenderer(32.f);

sf::FloatRect getPlayerHitbox(const sf::Sprite& player)
{
    auto box = player.getGlobalBounds();

    box.size *= 0.6f;
    box.position += box.size * 0.2f;

    return box;
}

bool checkWallCollision(const sf::FloatRect& box, const MapGenerator& map)
{
    int left = static_cast<int>(box.position.x) / 32;
    int right = static_cast<int>(box.position.x + box.size.x) / 32;
    int top = static_cast<int>(box.position.y) / 32;
    int bottom = static_cast<int>(box.position.y + box.size.y) / 32;

    for (int y = top; y <= bottom; y++)
    {
        for (int x = left; x <= right; x++)
        {
            if (x < 0 || y < 0 || x >= map.width || y >= map.height)
                continue;

            if (map.grid[y][x] == WALL)
                return true;
        }
    }
    return false;
}

void respawnPlayer(sf::Sprite& player, const MapGenerator& map, float tileSize)
{
    if (map.respawns.empty())
        return;

    const auto& spawn = map.respawns.front();

    player.setPosition({
        spawn.x * tileSize + tileSize / 2.f,
        spawn.y * tileSize + tileSize / 2.f
        });
}

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

void updatePlayer(
    sf::Sprite& player,
    sf::Vector2f movement,
    float dt,
    float speed,
    const MapGenerator& map)
{
    sf::Vector2f delta = movement * speed * dt;

    // X axis
    player.move({ delta.x, 0.f });
    if (checkWallCollision(getPlayerHitbox(player), map))
        player.move({ -delta.x, 0.f });

    // Y axis
    player.move({ 0.f, delta.y });
    if (checkWallCollision(getPlayerHitbox(player), map))
        player.move({ 0.f, -delta.y });
}


void updateGun(sf::Sprite& gun, sf::Sprite& player, sf::RenderWindow& window, float gunDistance)
{
    //calculating the vector of gun postion based on mouse movement
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    sf::Vector2f playerToMouse = worldPos - player.getPosition();
    //for sprite flipping use negative scale values
    //we will calculate the angle of rotation based on our position vector
    gun.setRotation(playerToMouse.angle());
    if (gun.getRotation().asDegrees() > 90.0f && gun.getRotation().asDegrees() < 270.0f) {
        gun.setScale(sf::Vector2f(1.f, -1.f));
        player.setScale(sf::Vector2f(-1.f, 1.f));
    }
    else{
        gun.setScale(sf::Vector2f(1.f, 1.f));
        player.setScale(sf::Vector2f(1.f, 1.f));
    }
    gun.setPosition(player.getPosition() + playerToMouse.normalized() * gunDistance);
}

void shoot(std::vector<Bullet>& bullets,
           const sf::Sprite& gun,
           const sf::Sprite& player,
           const sf::RenderWindow& window,
           const sf::Texture& bulletTexture)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    sf::Vector2f dir = worldPos - player.getPosition();
    dir = dir.normalized();
    

    bullets.emplace_back(
        bulletTexture,
        gun.getPosition(),
        dir * bulletSpeed
    );
    bullets.back().sprite.rotate(dir.angle());
}

bool bulletHitsWall(const Bullet& bullet, const MapGenerator& map)
{
    sf::Vector2f pos = bullet.sprite.getPosition();

    int tileX = static_cast<int>(pos.x) / 32;
    int tileY = static_cast<int>(pos.y) / 32;

    if (tileX < 0 || tileY < 0 ||
        tileX >= map.width || tileY >= map.height)
        return true; // poza map¹ = kasujemy

    return map.grid[tileY][tileX] == WALL;
}

void updateBullets(
    std::vector<Bullet>& bullets,
    float dt,
    const MapGenerator& map)
{
    for (size_t i = 0; i < bullets.size(); )
    {
        bullets[i].sprite.move(bullets[i].velocity * dt);

        if (bulletHitsWall(bullets[i], map))
        {
            bullets.erase(bullets.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}


void drawBullets(sf::RenderWindow& window, const std::vector<Bullet>& bullets)
{
    for (const auto& b : bullets)
        window.draw(b.sprite);
}


static void render(sf::RenderWindow& window, const sf::Sprite& player, const sf::Sprite& gun)
{
    window.clear();
    window.setView(m_view);
    window.draw(player);
    window.draw(gun);
    drawBullets(window, bullets);   
    m_enemyManager.draw(window);
    m_exitRenderer.draw(window, m_map);
    m_respawnRenderer.draw(window, m_map);
    m_renderer.draw(window);
    window.display();
}


int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); //wa¿ne ¿eby losowe genrowanie dzia³a³o

    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Simple call!");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("images/test-player.png")) {
        std::cerr << "Failed to load image\n";
    }
    sf::Sprite player(playerTexture);
    player.setOrigin(sf::Vector2f(playerTexture.getSize().x / 2.f, playerTexture.getSize().y / 2.f));    
    player.setPosition(sf::Vector2f(960.f, 540.f));

    sf::Texture gunTexture;
    if (!gunTexture.loadFromFile("images/gun.png")) {
        std::cerr << "Failed to load image\n";        
    }
    sf::Sprite gun(gunTexture);
    gun.setOrigin(sf::Vector2f(gunTexture.getSize().x / 2.f, gunTexture.getSize().y / 2.f));    

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("images/bullet.png")) {
        std::cerr << "Failed to load image\n";
    }

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("images/test-enemy.png")) {
        std::cerr << "Failed to load image\n";
    }
    //Enemy enemy(enemyTexture, { 960.f, 540.f });

    

    sf::Clock clock;
    const float speedMultiplier = 300.f;
    const float gunDistance = 15.0f;

    float totalWidth = 100 * 32.0f;
    float totalHeight = 100 * 32.0f;

    m_view.setSize({ 1920, 1080 });
    //m_view.zoom(0.2f);
    
    m_map.reset();
    respawnPlayer(player, m_map, 32.f);

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            //if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {

            //    if (k->code == sf::Keyboard::Key::Space) {
            //        m_map.reset();
            //        /*m_enemyManager.spawnFromMap(m_map, TILE_SIZE);*/

            //        /*for (auto& s : m_map.enemySpawns) {
            //            m_enemies.push_back({ s.x, s.y });
            //        }*/
            //    }

            //}
        }

        sf::Vector2f movement = getPlayerInput();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
            shootClock.getElapsedTime().asSeconds() > shootDelay)
        {            
            shoot(bullets, gun, player, window, bulletTexture);
            shootClock.restart();
        }
        //updateEnemy(enemy, dt);
        
        updatePlayer(player, movement, dt, speedMultiplier, m_map);
        m_view.setCenter(player.getPosition());
        updateGun(gun, player, window, gunDistance);
        updateBullets(bullets, dt, m_map);
        //drawEnemy(enemy, window);
        render(window, player, gun);
    }
}
