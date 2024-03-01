#pragma once

#include "level.h"
#include <math.h>

using namespace std;

class LevelGenerator {
private:
    int w, h;
    int **map;

    void FillWithWalls();
    void CreateRoom(int x, int y, int maxSize, int minSize, int &generatedRooms, vector<Pos> &spawners);
    void SurroundWithWalls();
    int HasNeighbour(int x, int y, int targetTile);
    bool CanBeDoor(int x, int y);
    void GetCorrDirectionAndLength(int &dir, int &len, int &currX, int &currY, const int maxLen);
    void GeneratePass(int &corrX, int &corrY, vector<Pos> &rooms, vector<Pos> &spawners);
    void CleanupDoors();
    void SetStartPoint(Level &level, vector<Pos> &rooms);
    void CleanupStrayWalls();
    int GetNeighbouringWalls(int x, int y) const;

    static double Lerp(double a, double b, double t) {
        return a + t * (b - a);
    }
    static double Dist(Pos &p1, Pos &p2) {
        int dx = p2.x - p1.x;
        int dy = p2.y - p1.y;
        return sqrt(dx * dx + dy * dy);
    }
    static int Sign(double a) {
        return a > 0 ? 1 : a < 0 ? -1 : 0;
    }
public:
    LevelGenerator();
    void Generate(Level &level);
};