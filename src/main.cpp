#include <SFML/Graphics.hpp>
#include <iostream>

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


void render(sf::RenderWindow& window, const sf::Sprite& player, const sf::Sprite& gun)
{
    window.clear();
    window.draw(player);
    window.draw(gun);
    window.display();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Simple call!");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("images/test-player.png");
    sf::Sprite player(playerTexture);
    player.setOrigin(sf::Vector2f(playerTexture.getSize().x / 2.f, playerTexture.getSize().y / 2.f));
    player.setScale(sf::Vector2f(20.f, 20.f));
    player.setPosition(sf::Vector2f(960.f, 540.f));

    sf::Texture gunTexture;
    gunTexture.loadFromFile("images/gun.png");
    sf::Sprite gun(gunTexture);
    gun.setOrigin(sf::Vector2f(gunTexture.getSize().x / 2.f, gunTexture.getSize().y / 2.f));
    gun.setScale(sf::Vector2f(15.f, 15.f));

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
        updatePlayer(player, movement, dt, speedMultiplier);
        updateGun(gun, player, window, gunDistance);
        render(window, player, gun);
    }
}
