#include "level.h"

Level::Level(int w, int h) : 
    entities(std::vector<Entity *>()),
    enemySpawnPositions(std::vector<Pos>())
    {
    this->w = w;
    this->h = h;
    map = new int *[w];
    memoryMap = new int *[w];
    for (int x = 0; x < w; x++) {
        map[x] = new int[h];
        memoryMap[x] = new int[h];
    }
    Clear();
}

Level::~Level() {
    for (int x = 0; x < w; x++) {
        delete[] map[x];
        delete[] memoryMap[x];
    }
    delete[] map;
    delete[] memoryMap;
}

const std::vector<Entity *> &Level::GetEntities() const { return entities; }
int **Level::GetMap() const { return map; }
int **Level::GetMemoryMap() const { return memoryMap; }
const int Level::GetTileAt(int x, int y) const { return map[x][y]; }
const int Level::GetWidth() const { return w; }
const int Level::GetHeight() const { return h; }
const int Level::GetSeed() const { return seed; }
const Pos Level::GetStart() const { return startPos; }
const Pos Level::GetEnd() const { return endPos; }
std::vector<Pos> &Level::GetSpawnerPositions() { 
    return enemySpawnPositions;
}

Entity *Level::GetEntityAtPos(Pos pos) const {
    for (Entity *e : entities) {
        if (e->GetPosition() == pos) {
            return e;
        }
    }
    return nullptr;
}

void Level::ClearLevel(Player *player) {
    for (Entity *e : entities) {
        if (e != player) {
            delete e;
        }
    }
    entities.clear();
    Clear();
}

void Level::RemoveEntity(Entity *e) {
    int i = 0;
    for (Entity *ve : entities) {
        if (ve == e) {
            entities.erase(entities.begin() + i);
            return;
        }
        i++;
    }
}

void Level::SetMap(int **map) { this->map = map; }
void Level::SetSeed(int seed) { this->seed = seed; }
void Level::SetStart(Pos start) { startPos = start; }
void Level::SetEnd(Pos end) { endPos = end; }

void Level::Clear() {
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            map[x][y] = 0;
            memoryMap[x][y] = 0;
        }
    }
}

bool Level::SpawnEntity(Entity *entity) {
    if (entity == nullptr)
        return false;
    entity->SetParentLevel(this);
    entities.push_back(entity);
    return true;
}
