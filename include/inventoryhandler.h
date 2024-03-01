#pragma once
#include "entities/player.h"

enum InventoryHandlerResult {
    Redraw,
    NoOp,
    Exit
};

class InventoryHandler {
private:
    int selectedIndex;
    int displayStartIndex;
    Inventory &inventory;

public:
    InventoryHandler(Inventory &inv);
    ~InventoryHandler();
    InventoryHandlerResult Process();
    int selected() const;
    int start() const;
};
