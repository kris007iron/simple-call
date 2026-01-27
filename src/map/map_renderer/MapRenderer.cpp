#include "MapRenderer.hpp"

MapRenderer::MapRenderer(const MapGenerator& m)
    : map(m),
    tileset(),
    sprite(tileset)
{
    if (tileset.loadFromFile("C:\\Users\\User\\Desktop\\studia\\simple-call\\assets\\jan.png")) {
        // Tekstura jest ju¿ powi¹zana, loadFromFile j¹ po prostu wype³ni danymi
    }
    initWallTiles();
}

void MapRenderer::initWallTiles() {
    sf::IntRect singleTile({ 0, 0 }, { 32, 32 });

    // Ka¿dy rodzaj œciany u¿ywa tej samej grafiki 32x32
    for (int i = 0; i < 16; ++i) {
        wallTiles[i] = singleTile;
    }
}

void MapRenderer::draw(sf::RenderWindow& window) {
    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            Tile t = map.grid[y][x];

            if (t == FLOOR) {
                // RYSOWANIE BIA£EJ POD£OGI
                sprite.setTextureRect(sf::IntRect()); // Czyœcimy obszar tekstury (brak grafiki)
                sprite.setColor(sf::Color::White);    // Ustawiamy kolor na bia³y
            }
            else {
                // RYSOWANIE ŒCIANY Z TEKSTURY 32x32
                sprite.setColor(sf::Color::White);    // Upewniamy siê, ¿e kolor jest bia³y (¿eby nie barwi³ tekstury)
                int mask = map.getWallMask(x, y);

                // Poniewa¿ masz teksturê 32x32, zawsze u¿ywamy tego samego prostok¹ta
                sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));
            }

            sprite.setPosition({
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE)
                });

            window.draw(sprite);
        }
    }
}