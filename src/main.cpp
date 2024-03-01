#include "game.h"

int main() {
    // init
    const int screenWidth = TERMINAL_WIDTH;
    const int screenHeight = TERMINAL_HEIGHT;

    initscr();
    resize_term(screenHeight, screenWidth);
    noecho();
    cbreak();
    curs_set(0);

    Game game = Game();

    std::ifstream menu("logo.txt");
    const int width = 90;
    const int start = screenWidth / 2 - width / 2;
    int y = 10;
    for (std::string line; std::getline(menu, line);) {
        move(y, start);
        printw(line.c_str());
        y++;
    }
    menu.close();

    move(30, 41);
    printw("Press any key to start");
    getch();
    clear();

    game.Init();

    // main loop
    while (game.Gameloop()) {
        game.Clear();
        game.Start();
    }

    game.QuitGame();

    endwin();
    return 0;
}

