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


    sf::Clock clock; //for a delta time between frames
    const float speedMulitplier = 300.0f;
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

        window.clear();
        window.draw(player);
        window.display();
    }
}