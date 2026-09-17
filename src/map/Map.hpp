#pragma once
#include <vector>
#include <string>

enum Tile { WALL, FLOOR };

struct Room {
    int x, y, w, h;
};

struct RoomPreset {
    int width;
    int height;
    std::vector<std::string> layout;
};

struct ExitPoint {
    int x, y;
};

struct SpawnPoint {
    int x, y;
    std::string type;
};

struct RespawnPoint {
    int x;
    int y;
};

class MapGenerator {
public:
    MapGenerator(int w, int h);

    // Dane publiczne
    std::vector<std::vector<Tile>> grid;
    std::vector<Room> rooms;
    std::vector<RoomPreset> library;
    std::vector<ExitPoint> exits;
    std::vector<SpawnPoint> enemySpawns;
    std::vector<RespawnPoint> respawns;

    int width;
    int height;

    void reset();
    int getWallMask(int x, int y) const;

private:
    bool exitPlaced = false;

    bool spawnPlaced = false;

    void loadPresets();
    void generate(int x, int y, int w, int h, int depth);
    void createRoom(int x, int y, int w, int h);
    void connectRooms(Room a, Room b);

    void drawHLine(int x1, int x2, int y);
    void drawVLine(int y1, int y2, int x);
};
