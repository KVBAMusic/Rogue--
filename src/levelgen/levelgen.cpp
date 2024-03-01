#include "levelgen/levelgen.h"
#include <time.h>
#include <conio.h>

LevelGenerator::LevelGenerator() {
}

void LevelGenerator::Generate(Level &level) {
    // 1. define a chance of generating a room C and a number of total rooms N
    // 2. define max length of a corridor segment L
    // 3. start at a random spot
    // 4. as long as there's less than N rooms repeat:
    //  4.1 pick a random direction
    //  4.2 pick a random length R (from 1 to L)
    //   4.2.1 if it's too long, generate R again
    //  4.3 go forward by R units
    //  4.4 try to generate a room
    //   4.4.1 if room was generated, pick a random tile in the room and mark it as spawner
    // 5. put TILE_DOORs where there's a TILE_WALL tile sandwiched between two TILE_FLOOR tiles
    // 6. Set starting point and ending point so that they are in the opposite sides of the map
    // 7. Clean up stray wall blocks
    w = level.GetWidth();
    h = level.GetHeight();
    map = level.GetMap();
    
    vector<Pos> allRooms = vector<Pos>();

    const int numPasses = 8;
    srand(level.GetSeed());
    
    int corrX = rand() % (w - 2) + 1;
    int corrY = rand() % (h - 2) + 1;
    for (int i = 0; i < numPasses; i++) {
        GeneratePass(corrX, corrY, allRooms, level.GetSpawnerPositions());
        do {
            corrX = rand() % (w - 2) + 1;
            corrY = rand() % (h - 2) + 1;
        } while (level.GetTileAt(corrX, corrY) != TILE_FLOOR);
    }
    CleanupDoors();
    FillWithWalls();
    SurroundWithWalls();
    CleanupStrayWalls();

    SetStartPoint(level, allRooms);
}

void LevelGenerator::GeneratePass(int &corrX, int &corrY, vector<Pos> &rooms, vector<Pos> &spawners) {
    const int roomCount = 2;
    const int maxRoomSize = 9;
    const int minRoomSize = 3;
    const int maxCorridorLength = 12;
    const int roomChance = 12; // room generated if rand() % roomChance == 0

    int generatedRooms = 0;
    int corridorLength;
    int corridorDirection;

    int dirX;
    int dirY;
    CreateRoom(corrX, corrY, maxRoomSize, minRoomSize, generatedRooms, spawners);
    rooms.push_back({corrX, corrY});
    while (generatedRooms < roomCount) {
        GetCorrDirectionAndLength(corridorDirection, corridorLength, corrX, corrY, maxCorridorLength);
        switch (corridorDirection) {
        case 0:
            dirX = 1;
            dirY = 0;
            break;
        case 1:
            dirX = 0;
            dirY = 1;
            break;
        case 2:
            dirX = -1;
            dirY = 0;
            break;
        case 3:
            dirX = 0;
            dirY = -1;
            break;
        default:
            break;
        }
        for (int i = 0; i < corridorLength; i++) {
            if (map[corrX][corrY] == TILE_WALL && map[corrX + dirX][corrY + dirY] != TILE_WALL) {
                map[corrX][corrY] = TILE_DOOR;
            }
            else if (map[corrX][corrY] == TILE_EMPTY)
                map[corrX][corrY] = TILE_FLOOR;
            corrX += dirX;
            corrY += dirY;
        }
        if (rand() % roomChance == 0) {
            CreateRoom(corrX, corrY, maxRoomSize, minRoomSize, generatedRooms, spawners);
            rooms.push_back({corrX, corrY});
        }
    }
}

void LevelGenerator::GetCorrDirectionAndLength(int &dir, int &len, int &currX, int &currY, const int maxLen) {
    int destX;
    int destY;
    while (1) {
        dir = rand() % 4;
        len = (rand() % maxLen) + 1;
        switch (dir) {
        case 0:
            destX = currX + len;
            destY = currY;
            break;
        case 1:
            destX = currX;
            destY = currY + len;
            break;
        case 2:
            destX = currX - len;
            destY = currY;
            break;
        case 3:
            destX = currX;
            destY = currY - len;
            break;
        default:
            break;
        }
        if ((destX > 1 && destX < w - 1) && (destY > 1 && destY < h - 1))
            break;
    }
}

void LevelGenerator::FillWithWalls() {
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (map[x][y] == TILE_EMPTY && HasNeighbour(x, y, TILE_FLOOR)) {
                map[x][y] = TILE_WALL;
            }
        }
    }
}

int LevelGenerator::HasNeighbour(int x, int y, int targetTile) {
    int out = 0;
    for (int _x = x - 1; _x <= x + 1; _x++) {
        for (int _y = y - 1; _y <= y + 1; _y++) {
            if (_x < 0 || _x >= w || _y < 0 || _y >= h) {
                continue;
            }
            out += map[_x][_y] == targetTile;
        }
    }
    return out;
}

void LevelGenerator::CreateRoom(int x, int y, int maxSize, int minSize, int &generatedRooms, vector<Pos> &spawners) {
    const int rw = rand() % (maxSize - minSize) + minSize;
    const int rh = rand() % (maxSize - minSize) + minSize;
    const int topLeftX = x - rw / 2;
    const int topLeftY = y - rh / 2;
    int spawnX;
    int spawnY;
    for (int i = 0; i < 3; i++) {
        spawnX = rand() % (rw - 1) + topLeftX;
        spawnY = rand() % (rh - 1) + topLeftY;
        spawners.push_back({spawnX, spawnY});
    }
    for (int _x = topLeftX; _x < topLeftX + rw; _x++) {
        for (int _y = topLeftY; _y < topLeftY + rh; _y++) {
            if (_x < 0 || _x >= w || _y < 0 || _y >= h) continue;
            map[_x][_y] = TILE_FLOOR;
        }
    }
    for (int _x = topLeftX - 1; _x < topLeftX + rw + 1; _x++) {
        for (int _y = topLeftY - 1; _y < topLeftY + rh + 1; _y++) {
            if (_x < 0 || _x >= w || _y < 0 || _y >= h) continue;
            if (_x == topLeftX - 1 || _x == topLeftX + rw || _y == topLeftY || _y == topLeftY + rh)
                map[_x][_y] = map[_x][_y] == TILE_FLOOR ? TILE_DOOR : TILE_WALL;
        }
    }
    // FillWithWalls();
    generatedRooms++;
}

void LevelGenerator::SurroundWithWalls() {
    for (int _x = 0; _x < w; _x++) {
        for (int _y = 0; _y < h; _y++) {
            if (_x == 0 || _x == w - 1 || _y == 0 || _y == h - 1) {
                map[_x][_y] = TILE_WALL;
            }
        }
    }
}

// The funky notation here is because it looks for the TILE_WALL tiles adjacent
// to two TILE_FLOOR/TILE_EMPTY tiles which have IDs of 0 and 1 respectively
// This means that if we come across those, after one right shift we should have 0.
bool LevelGenerator::CanBeDoor(int x, int y) {
    if (x <= 0 || x >= w - 1 || y <= 0 || y >= h - 1)
        return false;
    if (map[x-1][y] == TILE_WALL && map[x+1][y] == TILE_WALL) {
        return map[x][y - 1] >> 1 == 0 && map[x][y + 1] >> 1 == 0;
    }
    if (map[x][y-1] == TILE_WALL && map[x][y+1] == TILE_WALL) {
        return map[x - 1][y] >> 1 == 0 && map[x + 1][y] >> 1 == 0;
    }
    return false;
}

void LevelGenerator::CleanupDoors() {
    for (int x = 1; x < w - 1; x++) {
        for (int y = 1; y < h - 1; y++) {
            if (map[x][y] == TILE_DOOR && !CanBeDoor(x, y)) {
                map[x][y] = TILE_FLOOR;
            }
        }
    }
}

void LevelGenerator::SetStartPoint(Level &level, vector<Pos> &rooms) {
    // find 2 furthest awway rooms
    double maxDist = 0;
    double d;
    Pos start;
    Pos end;
    for (size_t a = 0; a < rooms.size() - 1; a++) {
        for (size_t b = a + 1; b < rooms.size(); b++) {
            d = Dist(rooms[a], rooms[b]);
            if (d > maxDist) {
                maxDist = d;
                start = rooms[a];
                end = rooms[b];
            }
        }
    }
    map[start.x][start.y] =  TILE_START;
    map[end.x][end.y] =  TILE_END;

    level.SetStart(start);
    level.SetEnd(end);
}

void LevelGenerator::CleanupStrayWalls() {
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (map[x][y] != TILE_WALL)
                continue;
            if (GetNeighbouringWalls(x, y) < 2) {
                map[x][y] = TILE_FLOOR;
            }
        }
    }
}

int LevelGenerator::GetNeighbouringWalls(int x, int y) const {
    int result = 0;

    if (y - 1 >= 0) {
        result += map[x][y - 1] / 2 == 1; // wall has id of 2, door has 3
    }
    if (y + 1 < h) {
        result += map[x][y + 1] / 2 == 1;
    }
    if (x - 1 >= 0) {
        result += map[x - 1][y] / 2 == 1;
    }
    if (x + 1 < w) {
        result += map[x + 1][y] / 2 == 1;
    }
    return result;
}