#include "MapRenderer.hpp"

MapRenderer::MapRenderer(const MapGenerator& m)
    : map(m),
    tileset(),
    sprite(tileset)
{
    if (tileset.loadFromFile("assets/jan.png")) {
        // Tekstura jest ju¿ powi¹zana, loadFromFile j¹ po prostu wype³ni danymi
    }
    initWallTiles();
}

void MapRenderer::initWallTiles() {
    auto rect = [](int x, int y) {
        return sf::IntRect({ x, y }, { 32, 32 });
        };

    wallTiles[0] = rect(0, 0);
    wallTiles[1] = rect(32, 0);
    wallTiles[2] = rect(64, 0);
    wallTiles[3] = rect(96, 0);
    wallTiles[4] = rect(0, 32);
    wallTiles[5] = rect(32, 32);
    wallTiles[6] = rect(64, 32);
    wallTiles[7] = rect(96, 32);
    wallTiles[8] = rect(0, 64);
    wallTiles[9] = rect(32, 64);
    wallTiles[10] = rect(64, 64);
    wallTiles[11] = rect(96, 64);
    wallTiles[12] = rect(0, 96);
    wallTiles[13] = rect(32, 96);
    wallTiles[14] = rect(64, 96);
    wallTiles[15] = rect(96, 96);
}

void MapRenderer::draw(sf::RenderWindow& window) {
    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            Tile t = map.grid[y][x];

            if (t == FLOOR) {
         
                sprite.setTextureRect(sf::IntRect({ 0, 128 }, { 32, 32 }));
            }
            else {
                int mask = map.getWallMask(x, y);
                if (wallTiles.contains(mask)) {
                    sprite.setTextureRect(wallTiles[mask]);
                }
            }

            sprite.setPosition({
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE)
                });

            window.draw(sprite);
        }
    }
}