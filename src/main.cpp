#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Simple call!");
    sf::Texture playerTexture;
    playerTexture.loadFromFile("images/test-player.png");
    sf::Sprite player(playerTexture);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(player);
        window.display();
    }
}