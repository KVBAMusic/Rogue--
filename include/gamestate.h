#pragma once
#include "level.h"

using namespace std;

struct GameState {
    vector<Level *> levels;
    uint levelNumber;
    Player *player;

    GameState() : levels(vector<Level *>()) {
        levelNumber = 0;
        player = nullptr;
    }

    Level &currentLevel() {
        return *levels[levelNumber];
    }
};