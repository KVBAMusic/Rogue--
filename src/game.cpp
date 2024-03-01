#include "game.h"
#include "inventoryhandler.h"

Game::Game() : 
  screenWidth(TERMINAL_WIDTH),
  screenHeight(TERMINAL_HEIGHT),
  levelGen(LevelGenerator()),
  renderer(TextRenderer()),
  state(GameState()),
  enemyPool(EntityPool<BaseEnemy>()),
  itemReg(ItemRegistry()) {}

Game::~Game() {

}

void Game::Init() {
    state.levels.push_back(MakeLevel());
    InitEntities();
}

// return 1 on restart; 0 on game quit;
int Game::Gameloop() {
    EntityAction pl = Ok;
    BaseEnemy *spawned;
    int counter = 1;

    renderer.DrawLevel(state);
    do {
        for (Entity *e : state.currentLevel().GetEntities()) {
            if (e == state.player) {
                if (state.player->Dead()) {
                    renderer.ShowGameOverScreen();
                    char result = getch();
                    return result != 'q';
                }
                pl = e->Tick();
                continue;
            }
            if (pl == NoMove) continue;
            if (e->Dead()) {
                state.currentLevel().RemoveEntity(e);
                delete e;
                continue;
            }
            e->Tick();
        }
        if (pl == Quit) {
            switch (renderer.DrawPauseMenu())
            {
            case -1:
                renderer.DrawLevel(state);
                break;
            case 0:
                return 1;
            case 1:
                return 0;
            }
        }
        if (pl == Ok) renderer.DrawLevel(state);

        counter -= 1;
        if (counter == 0) {
            counter = 200;
            spawned = EntityFactory(*enemyPool.SpawnRandom())
                          .SpawnAwayFromPlayer(state.currentLevel(), state.player)
                          .Return();
            if (state.currentLevel().GetEntities().size() > 15 || spawned->GetPosition() == Pos{-1, -1}) {
                delete spawned;
            }
            else {
                state.currentLevel().SpawnEntity(spawned);
            }
        }

        if (pl == NextLevel) {
            state.currentLevel().RemoveEntity(state.player);
            state.levelNumber++;
            if (state.levelNumber == state.levels.size()) {
                state.levels.push_back(MakeLevel());
            }
            state.player->SetPosition(state.currentLevel().GetStart());
            state.currentLevel().SpawnEntity(state.player);
            clear();
            renderer.DrawLevel(state);
            continue;
        }

        if (pl == PreviousLevel) {
            if (state.levelNumber == 0) {
                continue;
            }
            state.currentLevel().RemoveEntity(state.player);
            state.levelNumber--;
            state.player->SetPosition(state.currentLevel().GetEnd());
            state.currentLevel().SpawnEntity(state.player);
            clear();
            renderer.DrawLevel(state);
            continue;
        }
    } while (1);
    return 0;
}

void Game::QuitGame() {
    for (Level *l : state.levels) {
        l->ClearLevel(state.player);
        delete l;
    }

    delete state.player;
}

void Game::Clear() {
    state.currentLevel().RemoveEntity(state.player);
    state.player->SetParentLevel(nullptr);
    for (Level *l : state.levels) {
        l->ClearLevel(state.player);
        delete l;
    }
    state.levels.clear();
    state.levelNumber = 0;
}

void Game::Start() {
    state.levels.push_back(MakeLevel());
    state.player->SetEntityStats({20, 1, 0, 10, 7});
    state.player->SetPosition(state.currentLevel().GetStart());
    state.player->SetTarget(nullptr);
    state.player->GetInventory().Clear();
    state.player->GetInventory().AddItem(itemReg.healthPotion);
    state.player->GetInventory().AddItem(itemReg.healthPotion);
    state.currentLevel().SpawnEntity(state.player);
}

void Game::InitEntities() {
    
    // make player
    state.player = EntityFactory<Player>()
                        .SpawnAtLevelStart(state.currentLevel())
                        .SetName("Player")
                        .SetDisplayData('j', COLOR_BLACK, COLOR_RED)
                        .SetEntityStats({20, 1, 0, 10, 7})
                        .Return();
    state.player->GetInventory().AddItem(itemReg.healthPotion);
    state.player->GetInventory().AddItem(itemReg.healthPotion);
    state.player->SetInventoryCallback([this]() { 
        InventoryHandler handler = InventoryHandler(state.player->GetInventory());
        renderer.DrawPlayerInventory(state, handler);
        int result = 1;
        while (result) {
            switch (handler.Process()) {
            case Redraw:
                renderer.DrawPlayerInventory(state, handler);
                break;
            case NoOp:
                break;
            case Exit:
                result = 0;
                break;
            }
        }
        renderer.DrawLevel(state);
        });
    state.currentLevel().SpawnEntity(state.player);

    // make enemy source objects
    BaseEnemy *greenSlime = EntityFactory<BaseEnemy>()
                          .SetDisplayData('o', COLOR_GREEN, COLOR_BLACK)
                          .SetName("Green slime")
                          .SetEntityStats({2, 1, 0, 10, 4})
                          .Return();
    greenSlime->SetLootTable(itemReg.basicTable);
    enemyPool.AddEntity(greenSlime, 60);

    BaseEnemy *redSlime = EntityFactory<BaseEnemy>()
                          .SetDisplayData('o', COLOR_RED, COLOR_BLACK)
                          .SetName("Red slime")
                          .SetEntityStats({2, 1, 0, 10, 4})
                          .Return();
    redSlime->SetLootTable(itemReg.basicTable);
    enemyPool.AddEntity(redSlime, 60);

    BaseEnemy *yellowSlime = EntityFactory<BaseEnemy>()
                          .SetDisplayData('o', COLOR_YELLOW, COLOR_BLACK)
                          .SetName("Yellow slime")
                          .SetEntityStats({3, 1, 0, 10, 4})
                          .Return();
    yellowSlime->SetLootTable(itemReg.yellowSlimeTable);
    enemyPool.AddEntity(yellowSlime, 55);

    BaseEnemy *blueSlime = EntityFactory<BaseEnemy>()
                          .SetDisplayData('o', COLOR_CYAN, COLOR_BLACK)
                          .SetName("Blue slime")
                          .SetEntityStats({2, 1, 0, 10, 4})
                          .Return();
    blueSlime->SetLootTable(itemReg.basicTable);
    enemyPool.AddEntity(blueSlime, 60);

    BaseEnemy *purpleSlime = EntityFactory<BaseEnemy>()
                                 .SetDisplayData('O', COLOR_MAGENTA, COLOR_BLACK)
                                 .SetName("Purple slime")
                                 .SetEntityStats({5, 3, 0, 10, 6})
                                 .Return();
    purpleSlime->SetLootTable(itemReg.purpleSlimeTable);
    enemyPool.AddEntity(purpleSlime, 30);

    BaseEnemy *amethystGolem = EntityFactory<BaseEnemy>()
                                  .SetDisplayData('A', COLOR_BLACK, COLOR_MAGENTA)
                                  .SetName("Amethyst Golem")
                                  .SetEntityStats({10, 5, 3, 25, 8})
                                  .Return();
    amethystGolem->SetLootTable(itemReg.amethystGolemTable);
    enemyPool.AddEntity(amethystGolem, 5);

    BaseEnemy *amethystMinion = EntityFactory<BaseEnemy>()
                                    .SetDisplayData('a', COLOR_MAGENTA, COLOR_BLACK)
                                    .SetName("Amethyst minion")
                                    .SetEntityStats({6, 2, 0, 15, 7})
                                    .Return();
    amethystMinion->SetLootTable(itemReg.amethystMinionTable);
    enemyPool.AddEntity(amethystMinion, 20);
}


Level *Game::MakeLevel() {
    Level *level = new Level(LEVEL_WIDTH, LEVEL_HEIGHT);
    level->SetSeed(time(nullptr));
    levelGen.Generate(*level);
    return level;
}