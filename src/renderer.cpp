#include "renderer.h"
#include "gamelogs.h"
#include <pdcurses.h>

using namespace std;

TextRenderer::TextRenderer(int w, int h) {
    
    this->w = w;
    this->h = h;
    visibleMap = new int *[w];
    for (int x = 0; x < w; x++) {
        visibleMap[x] = new int[h];
    }
    ClearMap();

    start_color();
    // redefine colours
    init_color(COLOR_BLUE, 0, 150, 700);
    init_color(COLOR_GREEN, 250, 1000, 150);
    init_color(COLOR_YELLOW, 800, 600, 0);
    init_color(COLOR_RED, 700, 250, 180);

    // visible colours
    // id, foreground, background
    // initialise all possible colour pairs

    for (short bg = 0; bg < 8; bg++) {
        for (short fg = 0; fg < 8; fg++) {
            init_pair(bg * 8 + fg + 1, fg, bg);
        }
    }
}

TextRenderer::~TextRenderer() {
    for (int x = 0; x < w; x++) {
        delete[] visibleMap[x];
    }
    delete[] visibleMap;
}

const short GetPairIndex(const short fg, const short bg) {
    return bg * 8 + fg + 1;
}

void TextRenderer::ClearMap() {
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            visibleMap[x][y] = 0;
        }
    }
}

constexpr double TextRenderer::Rotate(double deg) {
    return deg / 180 * M_PI;
}

void TextRenderer::DrawLevel(GameState &state) {
    const char tiles[] = {' ', '.', '#', '!', '^', '%'};
    const short coloursVisible[] = {
        GetPairIndex(COLOR_BLACK, COLOR_BLACK),
        GetPairIndex(COLOR_WHITE, COLOR_BLACK),
        GetPairIndex(COLOR_WHITE, COLOR_BLACK),
        GetPairIndex(COLOR_BLACK, COLOR_YELLOW),
        GetPairIndex(COLOR_GREEN, COLOR_BLACK),
        GetPairIndex(COLOR_CYAN, COLOR_BLACK),
    };

    const short coloursMemory[] = {
        GetPairIndex(COLOR_BLUE, COLOR_BLACK),
        GetPairIndex(COLOR_BLUE, COLOR_BLACK),
        GetPairIndex(COLOR_BLUE, COLOR_BLACK),
        GetPairIndex(COLOR_BLACK, COLOR_BLUE),
        GetPairIndex(COLOR_BLUE, COLOR_BLACK),
        GetPairIndex(COLOR_BLUE, COLOR_BLACK),
    };
    ClearMap();

    const int w = state.currentLevel().GetWidth();
    const int h = state.currentLevel().GetHeight();

    const int NUM_RAYS = 180;
    const int RAY_LENGTH = state.player->GetStats().viewRadius;

    int **memMap = state.currentLevel().GetMemoryMap();

    // get fov
    double x, y;
    int rX, rY; // rounded coordinates
    int curr;
    double angle = 0;
    x = state.player->GetPosition().x;
    y = state.player->GetPosition().y;
    visibleMap[(int)x][(int)y] = state.currentLevel().GetTileAt(x, y);
    while (angle < 2 * M_PI) {
        x = state.player->GetPosition().x;
        y = state.player->GetPosition().y;
        for (int i = 0; i < RAY_LENGTH; i++) {
            x += sin(angle);
            y += cos(angle);
            rX = round(x);
            rY = round(y);
            curr = state.currentLevel().GetTileAt(rX, rY);
            visibleMap[rX][rY] = curr;
            memMap[rX][rY] = curr;
            if (curr == TILE_WALL || curr == TILE_DOOR)
                break;
        }
        angle += Rotate(360.0 / NUM_RAYS);
    }

    // render map
    clear();
    Entity *e;
    short colorIdx;
    char c;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            setsyx(y, x);
            curr = visibleMap[(int)x][(int)y]; // fine coz we using whole values
            colorIdx = coloursVisible[curr];
            e = state.currentLevel().GetEntityAtPos({(int)x, (int)y});
            c = tiles[curr];
            if (e != nullptr) {
                colorIdx = GetPairIndex(e->GetDisplayData().fg, e->GetDisplayData().bg);
                c = e->GetDisplayData().dispChar;
            }
            if (curr == 0) {
                curr = memMap[(int)x][int(y)];
                colorIdx = coloursMemory[curr];
                c = tiles[curr];
            }
            attron(COLOR_PAIR(colorIdx));
            mvaddch(y, x, c);
            attroff(COLOR_PAIR(colorIdx));
        }
    }

    //render ui
    DrawUI(state);

    refresh();
}

void TextRenderer::DrawUI(GameState &state) {
    short colorIdx;
    char c;
    //level number
    move(2, LEVEL_WIDTH + 2);
    attron(A_ITALIC);
    printw("Level %d", state.levelNumber + 1);
    attroff(A_ITALIC);

    //player hp
    const int playerHp = state.player->GetStats().health;
    const uint playerMaxHp = state.player->GetMaxHealth();

    move(5, LEVEL_WIDTH + 2);
    attron(COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK)));
    printw("HP: %d/%d", playerHp, playerMaxHp);
    attroff(COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK)));

    //hp bar
    DrawBar(LEVEL_WIDTH + 2, 6, 21, COLOR_RED, (double)playerHp / playerMaxHp);

    //player stats
    move(8, LEVEL_WIDTH + 2);
    printw("Stats:");
    DrawEntityStats(LEVEL_WIDTH + 2, 9, *state.player, COLOR_BLACK, COLOR_CYAN, A_BOLD);

    //new item
    if (Logger::instance().HasMessage()) {
        if (Logger::instance().GetMessage() == 1) {
            move(12, LEVEL_WIDTH + 2);
            attron(A_BOLD | A_ITALIC | COLOR_PAIR(GetPairIndex(COLOR_MAGENTA, COLOR_BLACK)));
            printw("Loot collected");
            attroff(A_BOLD | A_ITALIC | COLOR_PAIR(GetPairIndex(COLOR_MAGENTA, COLOR_BLACK)));
        }
    }

    //target
    const Attackable *target = state.player->GetTarget();
    if (target != nullptr) {
        int targetHp = target->GetStats().health;
        int targetMaxHp = target->GetMaxHealth();
        colorIdx = GetPairIndex(target->GetDisplayData().fg, target->GetDisplayData().bg);
        c = target->GetDisplayData().dispChar;

        move(25, LEVEL_WIDTH + 2);
        printw("Target entity: ");
        attron(COLOR_PAIR(colorIdx));
        mvaddch(27, LEVEL_WIDTH + 2, c);
        attroff(COLOR_PAIR(colorIdx));
        move(28, LEVEL_WIDTH + 2);
        printw("%s", target->name);

        move(30, LEVEL_WIDTH + 2);
        attron(COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK)));
        printw("HP: %d/%d", targetHp, targetMaxHp);
        attroff(COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK)));
        DrawBar(LEVEL_WIDTH + 2, 31, 21, COLOR_RED, (double)targetHp / targetMaxHp);
        DrawEntityStats(LEVEL_WIDTH + 2, 32, *target, COLOR_BLACK, COLOR_CYAN, A_BOLD);
    }
}

// 1 - do nothing, 0 - restart, -1 - quit
int TextRenderer::DrawPauseMenu() {
    int selectedIdx = 0;

    chtype title = COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK)) | A_BOLD | A_ITALIC;
    chtype normal = COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK));
    chtype selected = COLOR_PAIR(GetPairIndex(COLOR_WHITE, COLOR_BLACK)) | A_BOLD;
    do {
        clear();
        move(LEVEL_HEIGHT - 10, 5);
        attron(title);
        printw("PAUSE");
        attroff(title);

        move(LEVEL_HEIGHT - 8, 5);
        attron(selectedIdx == 0 ? selected : normal);
        printw("Continue");
        attroff(selectedIdx == 0 ? selected : normal);

        move(LEVEL_HEIGHT - 7, 5);
        attron(selectedIdx == 1 ? selected : normal);
        printw("Restart game");
        attroff(selectedIdx == 1 ? selected : normal);

        move(LEVEL_HEIGHT - 6, 5);
        attron(selectedIdx == 2 ? selected : normal);
        printw("Quit");
        attroff(selectedIdx == 2 ? selected : normal);
        switch (getch()) {
            case 'w':
                selectedIdx -= 1;
                if (selectedIdx < 0) {
                    selectedIdx = 2;
                }
                break;
            case 's':
                selectedIdx = (selectedIdx + 1) % 3;
                break;
            case ' ':
                return selectedIdx - 1;
        }
    } while (1);
}

void TextRenderer::DrawBar(int x, int y, int length, short colour, double value) {
    int barLength = 21;
    int relativeHp = (int)(value * barLength);
    int endOfBar = 0;
    move(y, x);
    attron(COLOR_PAIR(GetPairIndex(colour, colour)));
    for (int i = 0; i < barLength; i++) {
        if (i > relativeHp && !endOfBar) {
            endOfBar = 1;
            attroff(COLOR_PAIR(GetPairIndex(colour, colour)));
            attron(COLOR_PAIR(GetPairIndex(colour, COLOR_BLACK)));
        }
        printw("_");
    }
    if (endOfBar) {
        attroff(COLOR_PAIR(GetPairIndex(colour, COLOR_BLACK)));
    }
    else {
        attroff(COLOR_PAIR(GetPairIndex(colour, colour)));
    }
}

void TextRenderer::DrawItemStats(int x, int y, const StatModifiers &stats, short bg, short sep, uint attr) {
    chtype attrHealth = attr | COLOR_PAIR(GetPairIndex(COLOR_RED, bg));
    chtype attrStrength = attr | COLOR_PAIR(GetPairIndex(COLOR_GREEN, bg));
    chtype attrDefence = attr | COLOR_PAIR(GetPairIndex(COLOR_YELLOW, bg));
    chtype attrSeparator = attr | COLOR_PAIR(GetPairIndex(sep, bg));

    move(y, x);

    attron(attrHealth);
    printw("%d ", stats.health);
    attroff(attrHealth);

    attron(attrSeparator);
    printw("/ ");
    attroff(attrSeparator);

    attron(attrStrength);
    printw("%d ", stats.strength);
    attroff(attrStrength);

    attron(attrSeparator);
    printw("/ ");
    attroff(attrSeparator);

    attron(attrDefence);
    printw("%d ", stats.defence);
    attroff(attrDefence);

    attron(attrSeparator);
    printw("/ ");
    attroff(attrSeparator);

    attron(attrHealth);
    printw("%d", stats.maxHealth);
    attroff(attrHealth);
}

void TextRenderer::DrawEntityStats(int x, int y, const Attackable &e, short bg, short sep, uint attr) {
    chtype attrHealth = attr | COLOR_PAIR(GetPairIndex(COLOR_RED, bg));
    chtype attrStrength = attr | COLOR_PAIR(GetPairIndex(COLOR_GREEN, bg));
    chtype attrDefence = attr | COLOR_PAIR(GetPairIndex(COLOR_YELLOW, bg));
    chtype attrSeparator = attr | COLOR_PAIR(GetPairIndex(sep, bg));

    EntityStats stats = e.GetStats();

    move(y, x);

    attron(attrHealth);
    printw("%d ", e.GetMaxHealth());
    attroff(attrHealth);

    attron(attrSeparator);
    printw("/ ");
    attroff(attrSeparator);

    attron(attrStrength);
    printw("%d ", stats.strength);
    attroff(attrStrength);

    attron(attrSeparator);
    printw("/ ");
    attroff(attrSeparator);

    attron(attrDefence);
    printw("%d", stats.defence);
    attroff(attrDefence);
}

void TextRenderer::ShowGameOverScreen() {
    clear();
    attron(COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK)));
    move(30, 15);
    printw("Game over");
    move(31, 15);
    printw("Press any key to restart");
    move(32, 15);
    printw("Press Q to quit");
    attroff(COLOR_PAIR(GetPairIndex(COLOR_RED, COLOR_BLACK)));
    refresh();
}

void TextRenderer::DrawPlayerInventory(GameState &state, InventoryHandler &handler) {
    Inventory &inv = state.player->GetInventory();

    clear();

    // tooltip at the bottom
    move(38, 3);
    printw("W/S - Navigate    Space - Use/Equip    Q - Back");

    // title
    move(1, 5);
    attron(A_BOLD | A_ITALIC);
    printw("-+ INVENTORY +-");
    attroff(A_BOLD | A_ITALIC);

    // list of items
    if (inv.GetSize() == 0) {
        move(3, 3);
        printw("(empty)");
    }
    else {
        int startIdx = handler.start();
        bool selected;
        InvNode *item;
        for (int line = 3; line < 37 && startIdx < inv.GetSize(); line++) {
            item = inv.GetAtIdx(startIdx);
            selected = startIdx == handler.selected();

            move(line, 3);
            if (selected) attron(COLOR_PAIR(GetPairIndex(COLOR_CYAN, COLOR_BLACK)));
            printw("%d. %s x%d", startIdx + 1, item->GetName(), item->count);
            if (item->equipped) {
                printw(" (equipped)");
            }
            if (selected) attroff(COLOR_PAIR(GetPairIndex(COLOR_CYAN, COLOR_BLACK)));
            
            const Consumable *c = item->c;
            if (c != nullptr) {
                DrawItemStats(45, line, c->GetModifiers(), COLOR_BLACK, selected ? COLOR_CYAN : COLOR_WHITE, A_BOLD);
            }
            const Wearable *w = item->w;
            if (w != nullptr) {
                DrawItemStats(45, line, w->GetModifiers(), COLOR_BLACK, selected ? COLOR_CYAN : COLOR_WHITE, A_BOLD);
            }
            
            startIdx++;
        }
    }
    DrawUI(state);
    refresh();
}