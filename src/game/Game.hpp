#pragma once
#include <SFML/Graphics.hpp>
#include "../map/Map.hpp"
#include "../map/map_renderer/MapRenderer.hpp"
#include "../enemy/EnemyManager.hpp"
#include "../map/exit_renderer/ExitRenderer.hpp"
#include "../map/respawn_renderer/RespawnRenderer.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void drawExits();

    struct Enemy {
        int x;
        int y;
    };

    std::vector<Enemy> m_enemies;

    static constexpr float TILE_SIZE = 32.0f;
    static constexpr int MAP_W = 100;
    static constexpr int MAP_H = 100;

    sf::View m_view;

    sf::RenderWindow m_window;
    MapGenerator m_map;
    MapRenderer m_renderer;
    EnemyManager m_enemyManager;
    ExitRenderer m_exitRenderer;
    RespawnRenderer m_respawnRenderer;

};
