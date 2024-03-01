#pragma once
#include "entities/testenemy.h"
#include <list>

class Level {
private:
    int w, h;
    int **map;
    int **memoryMap;
    std::vector<Entity *> entities;
    std::vector<Pos> enemySpawnPositions;

    int seed;
    Pos startPos, endPos;

    void Clear();

public:
    Level(int w, int h);
    ~Level();

    const std::vector<Entity *> &GetEntities() const;
    int **GetMap() const;
    int **GetMemoryMap() const;
    const int GetTileAt(int x, int y) const;
    const int GetWidth() const;
    const int GetHeight() const;
    const int GetSeed() const;
    const Pos GetStart() const;
    const Pos GetEnd() const;
    std::vector<Pos> &GetSpawnerPositions();
    Entity *GetEntityAtPos(Pos pos) const;
    void ClearLevel(Player *player);
    void RemoveEntity(Entity *e);

    void SetMap(int **map);
    void SetSeed(int seed);
    void SetStart(Pos start);
    void SetEnd(Pos end);
    bool SpawnEntity(Entity *entity);
};