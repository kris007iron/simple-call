#include "Map.hpp"
#include <random>
#include <algorithm>

MapGenerator::MapGenerator(int w, int h) : width(w), height(h) {
    loadPresets();
    reset();
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

    library.push_back({ 11, 11, {
    "###########",
    "#....*....#",
    "#.........#",
    "#..###.###.#",
    "#..#.....#.#",
    "#..###.###.#",
    "#.........#",
    "#....*....#",
    "#.........#",
    "#....*....#",
    "###########"
    } });

    library.push_back({ 9, 9, {
    "#########",
    "#*.....*#",
    "#.#####.#",
    "#.......#",
    "#.#####.#",
    "#.......#",
    "#.#####.#",
    "#*.....*#",
    "#########"
} });

    library.push_back({ 13, 7, {
    "#############",
    "#*....#....*#",
    "#####.#.#####",
    "#...........#",
    "#####.#.#####",
    "#*....#....*#",
    "#############"
} });

    library.push_back({ 7, 7, {
    "#######",
    "#..*..#",
    "#.###.#",
    "#*...*#",
    "#.###.#",
    "#..*..#",
    "#######"
} });

    library.push_back({ 11, 5, {
    "###########",
    "#..*...*..#",
    "#.........#",
    "#..*...*..#",
    "###########"
} });

    library.push_back({ 9, 7, {
    "#########",
    "#..*....#",
    "#.#####.#",
    "#..*....#",
    "#.#####.#",
    "#....*..#",
    "#########"
} });

    library.push_back({ 9, 9, {
    "#########",
    "#..***..#",
    "#.#####.#",
    "#*.....*#",
    "#*.....*#",
    "#*.....*#",
    "#.#####.#",
    "#..***..#",
    "#########"
} });

    library.push_back({ 9, 7, {
    "#########",
    "#*.....*#",
    "#...E...#",
    "#.......#",
    "#*.....*#",
    "#.......#",
    "#########"
} });

    library.push_back({ 5, 5, {
    ".....",
    "..#..",
    ".*.*.",
    "..#..",
    "....."
} });
    library.push_back({ 5, 5, {
        ".#.#.",
        "#...#",
        "*...*",
        "#...#",
        ".#.#."
    } });
    library.push_back({ 5, 5, {
        ".*.*.",
        "..#..",
        "..#..",
        ".*.*.",
        "....."
    } });
    library.push_back({ 5, 5, {
        ".....",
        ".***.",
        ".*#*.",
        ".***.",
        "....."
    } });

    library.push_back({ 7, 7, {
        "#######",
        "#*...*#",
        "#..#..#",
        "#..*..#",
        "#..#..#",
        "#*...*#",
        "#######"
    } });
    library.push_back({ 7, 7, {
        "#..#..#",
        ".#.*.#.",
        "..*.*..",
        ".#.*.#.",
        "#..#..#",
        ".......",
        "#..*..#"
    } });
    library.push_back({ 7, 7, {
        ".......",
        "..###..",
        ".*...*.",
        ".#.*.#.",
        ".*...*.",
        "..###..",
        "......."
    } });
    library.push_back({ 7, 7, {
        "#*#*#*#",
        ".......",
        "..*.*..",
        ".......",
        "..*.*..",
        ".......",
        "#*#*#*#"
    } });

    library.push_back({ 9, 9, {
        "#########",
        "#*.....*#",
        "#..###..#",
        "#.*...*.#",
        "#..###..#",
        "#.*...*.#",
        "#..###..#",
        "#*.....*#",
        "#########"
    } });
    library.push_back({ 9, 9, {
        "#########",
        "#..*.*..#",
        "#.*.#.*.#",
        "#.*.#.*.#",
        "#..*.*..#",
        "#.*.#.*.#",
        "#.*.#.*.#",
        "#..*.*..#",
        "#########"
    } });
    library.push_back({ 9, 9, {
        "#########",
        "#..*.*..#",
        "#..###..#",
        "#.*...*.#",
        "#..###..#",
        "#.*...*.#",
        "#..###..#",
        "#..*.*..#",
        "#########"
    } });
    library.push_back({ 9, 9, {
        "#########",
        "#*.*.*.*#",
        "#.......#",
        "#.*.*.*.#",
        "#.......#",
        "#.*.*.*.#",
        "#.......#",
        "#*.*.*.*#",
        "#########"
    } });

    library.push_back({ 11, 7, {
        "###########",
        "#*..#..*..#",
        "#.........#",
        "#..###....#",
        "#.........#",
        "#*.......*#",
        "###########"
    } });
    library.push_back({ 11, 7, {
        "###########",
        "#.*...*...#",
        "#..###..#.#",
        "#.........#",
        "#..#..###.#",
        "#*...*.*..#",
        "###########"
    } });
    library.push_back({ 11, 7, {
        "###########",
        "#*.......*#",
        "#..###....#",
        "#.*...*...#",
        "#....###..#",
        "#*.......*#",
        "###########"
    } });
    library.push_back({ 11, 7, {
        "###########",
        "#..*.*.*..#",
        "#.........#",
        "#.*.....*.#",
        "#.........#",
        "#..*.*.*..#",
        "###########"
    } });

    library.push_back({ 13, 9, {
        "#############",
        "#*..#...*...#",
        "#....###....#",
        "#.*.......*.#",
        "#....###....#",
        "#.*.......*.#",
        "#....###....#",
        "#*...*...#..#",
        "#############"
    } });
    library.push_back({ 13, 9, {
        "#############",
        "#.*.*.*.*.*.#",
        "#...........#",
        "#.*.*.*.*.*.#",
        "#...........#",
        "#.*.*.*.*.*.#",
        "#...........#",
        "#.*.*.*.*.*.#",
        "#############"
    } });
    library.push_back({ 13, 9, {
        "#############",
        "#*...*...*...#",
        "#..###..###..#",
        "#.*.......*..#",
        "#..###..###..#",
        "#.*.......*..#",
        "#..###..###..#",
        "#*...*...*...#",
        "#############"
    } });
    library.push_back({ 13, 9, {
        "#############",
        "#..*.*.*.*..#",
        "#...........#",
        "#.*.......*.#",
        "#...........#",
        "#.*.......*.#",
        "#...........#",
        "#..*.*.*.*..#",
        "#############"
    } });

}

void MapGenerator::generate(int x, int y, int w, int h, int depth) {
    const int MIN_SIZE = 6;
    const int MAX_PREFABS_PER_ROOM = 2;

    if (depth <= 0 || w < MIN_SIZE * 2 || h < MIN_SIZE * 2) {
        int numPrefabs = 1 + rand() % MAX_PREFABS_PER_ROOM;
        for (int i = 0; i < numPrefabs; ++i) {
            createRoom(x, y, w, h);
        }
        return;
    }

    bool splitH = (rand() % 2 == 0);
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
        int rx = x + rand() % (w - p.width + 1);
        int ry = y + rand() % (h - p.height + 1);

        for (int py = 0; py < p.height; ++py) {
            for (int px = 0; px < p.width; ++px) {
                char c = p.layout[py][px];
                int gx = ry + py;
                int gy = rx + px;

                if (gx < 0 || gx >= height || gy < 0 || gy >= width)
                    continue;

                switch (c) {
                case '.':
                    grid[gx][gy] = FLOOR;
                    break;
                case '#':
                    grid[gx][gy] = WALL;
                    break;
                case '*':
                    grid[gx][gy] = FLOOR;
                    enemySpawns.push_back({ gy, gx, "enemy" });
                    break;
                default:
                    grid[gx][gy] = WALL;
                    break;
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

int MapGenerator::getWallMask(int x, int y) const {
    if (grid[y][x] == FLOOR) return -1;
    int m = 0;
    if (y > 0 && grid[y - 1][x] == WALL) m += 1;
    if (y < height - 1 && grid[y + 1][x] == WALL) m += 2;
    if (x > 0 && grid[y][x - 1] == WALL) m += 4;
    if (x < width - 1 && grid[y][x + 1] == WALL) m += 8;
    return m;
}

void MapGenerator::reset() {
    grid.assign(height, std::vector<Tile>(width, WALL));
    rooms.clear();
    enemySpawns.clear();
    exits.clear();
    respawns.clear();
    exitPlaced = false;
    spawnPlaced = false;

    generate(0, 0, width, height, 5);

    for (size_t i = 0; i < rooms.size() - 1; ++i)
        connectRooms(rooms[i], rooms[i + 1]);

    if (!rooms.empty()) {
        int startRoomIndex = rand() % rooms.size();
        const Room& startRoom = rooms[startRoomIndex];
        int spawnX = startRoom.x + startRoom.w / 2;
        int spawnY = startRoom.y + startRoom.h / 2;
        respawns.push_back({ spawnX, spawnY });
        spawnPlaced = true;

        int exitRoomIndex;
        do {
            exitRoomIndex = rand() % rooms.size();
        } while (exitRoomIndex == startRoomIndex);

        const Room& exitRoom = rooms[exitRoomIndex];
        int ex = exitRoom.x + exitRoom.w / 2;
        int ey = exitRoom.y + exitRoom.h / 2;
        exits.push_back({ ex, ey });
        grid[ey][ex] = FLOOR;
        exitPlaced = true;
    }
}






