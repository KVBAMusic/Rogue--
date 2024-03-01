#pragma once

#include "levelgen/levelgen.h"
#include "renderer.h"
#include "entities/entityspawner.h"
#include "entities/entitypool.h"
#include "items/itemregistry.h"
#include <pdcurses.h>
#include <time.h>
#include <fstream>
#include <string>

class Game {
private:
    const int screenWidth;
    const int screenHeight;

    LevelGenerator levelGen;
    TextRenderer renderer;
    GameState state;
    EntityPool<BaseEnemy> enemyPool;
    ItemRegistry itemReg;

    Level *MakeLevel();
    void InitEntities();

public:
    Game();
    ~Game();

    void Init();
    void Start();
    void Clear();
    void QuitGame();
    int Gameloop();
};