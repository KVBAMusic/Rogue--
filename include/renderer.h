#pragma once
#include "gamestate.h"
#include "inventoryhandler.h"

class TextRenderer {
private:
    int w, h;
    int **visibleMap;
    void ClearMap();
    constexpr double Rotate(double deg);
    void DrawBar(int x, int y, int length, short colour, double value);
    void DrawItemStats(int x, int y, const StatModifiers &stats, short bg, short sep, uint attr);
    void DrawEntityStats(int x, int y, const Attackable &e, short bg, short sep, uint attr);
    void DrawUI(GameState &state);

public:
    TextRenderer(int w = LEVEL_WIDTH, int h = LEVEL_HEIGHT);
    ~TextRenderer();
    void DrawLevel(GameState &state);
    void ShowGameOverScreen();
    void DrawPlayerInventory(GameState &state, InventoryHandler &handler);
    int DrawPauseMenu();
};

inline const short GetPairIndex(const short fg, const short bg);