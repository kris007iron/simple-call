#pragma once
#include <SFML/Graphics.hpp>
#include "../map/Map.hpp"
#include "../map/map_renderer/MapRenderer.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    static constexpr float TILE_SIZE = 32.0f;
    static constexpr int MAP_W = 60;
    static constexpr int MAP_H = 50;

    sf::RenderWindow m_window;
    MapGenerator m_map;
    MapRenderer m_renderer;
};
