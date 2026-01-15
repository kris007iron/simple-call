#include "Map.hpp"
#include <random>
#include <algorithm>

MapGenerator::MapGenerator(int w, int h) : width(w), height(h) {
    loadPresets();
    reset();
}

void MapGenerator::reset() {
    grid.assign(height, std::vector<Tile>(width, WALL));
    rooms.clear();

    generate(0, 0, width, height, 5);

    for (size_t i = 0; i < rooms.size() - 1; ++i) {
        connectRooms(rooms[i], rooms[i + 1]);
    }
}

void MapGenerator::loadPresets() {
   
    library.push_back({ 11, 11, {
        "...........",
        ".#.......#.",
        "....#.#....",
        "...........",
        "..#.....#..",
        ".....#.....",
        "..#.....#..",
        "...........",
        "....#.#....",
        ".#.......#.",
        "..........."
    } });

    library.push_back({ 9, 7, {
        ".........",
        "#######..",
        ".........",
        "..#######",
        ".........",
        "#######..",
        "........."
    } });

    library.push_back({ 7, 7, {
        "..###..",
        ".#...#.",
        "#.....#",
        "#..#..#",
        "#.....#",
        ".#...#.",
        "..###.."
    } });

    library.push_back({ 11, 5, {
        "...........",
        ".#.#.#.#.#.",
        "...........",
        ".#.#.#.#.#.",
        "..........."
    } });

    library.push_back({ 9, 9, {
        ".........",
        ".#####.#.",
        ".......#.",
        ".#.#####.",
        ".#.......",
        ".#####.#.",
        ".......#.",
        ".#.#####.",
        "........."
    } });

    library.push_back({ 5, 5, {
        ".....",
        "..#..",
        ".#.#.",
        "..#..",
        "....."
    } });
}

void MapGenerator::generate(int x, int y, int w, int h, int depth) {
    const int MIN_SIZE = 10;

    // stop ¿eby dzielenia przez zero nie by³o
    if (depth <= 0 || w < MIN_SIZE * 2 || h < MIN_SIZE * 2) {
        createRoom(x, y, w, h);
        return;
    }

    bool splitH = (rand() % 2 == 0);
    // Zachowanie proporcji (¿eby pokoje nie by³y zbyt w¹skie
    if (w > h && (float)w / h >= 1.25) splitH = false;
    else if (h > w && (float)h / w >= 1.25) splitH = true;

    if (splitH) {
        int available = h - 2 * MIN_SIZE;
        int split = MIN_SIZE + (available > 0 ? rand() % (available + 1) : 0);
        generate(x, y, w, split, depth - 1);
        generate(x, y + split, w, h - split, depth - 1);
    }
    else {
        int available = w - 2 * MIN_SIZE;
        int split = MIN_SIZE + (available > 0 ? rand() % (available + 1) : 0);
        generate(x, y, split, h, depth - 1);
        generate(x + split, y, w - split, h, depth - 1);
    }
}

void MapGenerator::createRoom(int x, int y, int w, int h) {
    if (library.empty()) return;

    int index = rand() % library.size();
    const auto& p = library[index];

    if (p.width <= w && p.height <= h) {
        int rx = x + (w - p.width) / 2;
        int ry = y + (h - p.height) / 2;

        for (int py = 0; py < p.height; ++py) {
            for (int px = 0; px < p.width; ++px) {

                if (p.layout[py][px] == '.') {
                    grid[ry + py][rx + px] = FLOOR;
                }
                else {
                    grid[ry + py][rx + px] = WALL;
                }
            }
        }
        rooms.push_back({ rx, ry, p.width, p.height });
    }
}

void MapGenerator::connectRooms(Room a, Room b) {
    int sx = a.x + a.w / 2, sy = a.y + a.h / 2;
    int ex = b.x + b.w / 2, ey = b.y + b.h / 2;

    if (rand() % 2 == 0) {
        drawHLine(sx, ex, sy);
        drawVLine(sy, ey, ex);
    }
    else {
        drawVLine(sy, ey, sx);
        drawHLine(sx, ex, ey);
    }
}

void MapGenerator::drawHLine(int x1, int x2, int y) {
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
        if (y >= 0 && y < height && x >= 0 && x < width) grid[y][x] = FLOOR;
}

void MapGenerator::drawVLine(int y1, int y2, int x) {
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
        if (y >= 0 && y < height && x >= 0 && x < width) grid[y][x] = FLOOR;
}

// DODAJ 'const' TUTAJ:
int MapGenerator::getWallMask(int x, int y) const {
    if (grid[y][x] == FLOOR) return -1;
    int m = 0;
    if (y > 0 && grid[y - 1][x] == WALL) m += 1;
    if (y < height - 1 && grid[y + 1][x] == WALL) m += 2;
    if (x > 0 && grid[y][x - 1] == WALL) m += 4;
    if (x < width - 1 && grid[y][x + 1] == WALL) m += 8;
    return m;
}