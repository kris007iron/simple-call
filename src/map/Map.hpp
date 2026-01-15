#pragma once
#include <vector>
#include <string>

// Podstawowe typy danych u¿ywane w ca³ym projekcie
enum Tile { WALL, FLOOR };

struct Room {
    int x, y, w, h;
};

struct RoomPreset {
    int width;
    int height;
    std::vector<std::string> layout;
};

class MapGenerator {
public:
    // Dane publiczne, do których dostêp bêdzie mia³a klasa Game
    std::vector<std::vector<Tile>> grid;
    std::vector<Room> rooms;
    std::vector<RoomPreset> library;

    int width;
    int height;

    // Konstruktor i g³ówne metody
    MapGenerator(int w, int h);
    void reset();
    int getWallMask(int x, int y) const;

private:
    void loadPresets();
    void generate(int x, int y, int w, int h, int depth);
    void createRoom(int x, int y, int w, int h);
    void connectRooms(Room a, Room b);

    //pomocniczne do rysowania
    void drawHLine(int x1, int x2, int y);
    void drawVLine(int y1, int y2, int x);
};