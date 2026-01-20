#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Simple call!");
    sf::Texture playerTexture;
    playerTexture.loadFromFile("images/test-player.png");
    sf::Sprite player(playerTexture);
    player.setOrigin(sf::Vector2f(
        playerTexture.getSize().x / 2.f,
        playerTexture.getSize().y / 2.f)
    );
    player.setScale(sf::Vector2f(20.f, 20.f));
    player.setPosition(sf::Vector2f(960.f, 540.f));


    sf::Texture gunTexture;
    gunTexture.loadFromFile("images/gun.png");
    sf::Sprite gun(gunTexture);
    gun.setOrigin(sf::Vector2f(
        gunTexture.getSize().x / 2.f,
        gunTexture.getSize().y / 2.f)
    );
    gun.setScale(sf::Vector2f(15.f, 15.f));
    gun.setPosition(sf::Vector2f(960.f/2.f, 540.f/2.f));

    sf::Clock clock; //for a delta time between frames
    const float speedMulitplier = 300.0f;
    const float gunDistance = 50.0f;
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        //input from user
        sf::Vector2f vRequestedPlayerMovement(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
            vRequestedPlayerMovement += sf::Vector2f(0.0f, -1.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            vRequestedPlayerMovement += sf::Vector2f(1.0f, 0.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            vRequestedPlayerMovement += sf::Vector2f(0.0f, 1.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            vRequestedPlayerMovement += sf::Vector2f(-1.0f, 0.0f);
        }

        player.move(vRequestedPlayerMovement*deltaTime.asSeconds() * speedMulitplier);

        //to draw correclty we need to perform offest after player was moved
        sf::Vector2f vMousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
        sf::Vector2f vPlayerToMouse = vMousePosition - player.getPosition();
        //get a vector from mouse to player normalize it and multiply to chose distance

        gun.setPosition(player.getPosition() + (vPlayerToMouse.normalized() * gunDistance));


        window.clear();
        window.draw(player);
        window.draw(gun);
        window.display();
    }
}