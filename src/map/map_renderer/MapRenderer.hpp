#pragma once
#include <SFML/Graphics.hpp>
#include "../Map.hpp"
#include <unordered_map>

class MapRenderer {
public:
    static constexpr int TILE_SIZE = 32;

    MapRenderer(const MapGenerator& map);
    void draw(sf::RenderWindow& window);

private:
    const MapGenerator& map;
    sf::Texture tileset;
    sf::Sprite sprite;
    std::unordered_map<int, sf::IntRect> wallTiles;

    void initWallTiles();
};