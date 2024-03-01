#include "inventoryhandler.h"
#include <pdcurses.h>

InventoryHandler::InventoryHandler(Inventory &inv) : inventory(inv) {
    selectedIndex = 0;
    displayStartIndex = 0;
}

InventoryHandler::~InventoryHandler() {
}

InventoryHandlerResult InventoryHandler::Process() {
    uint size = inventory.GetSize();
    if (selectedIndex >= size) {
        selectedIndex = size - 1;
    } 
    int highBound = displayStartIndex + 36;
    char c = getch();
    switch (c) {
    case 'w':
        if (selectedIndex > 0) {
            selectedIndex--;
            if (selectedIndex < displayStartIndex) {
                displayStartIndex--;
            }
            return Redraw;
        }
        return NoOp;
    case 's':
        if (selectedIndex < inventory.GetSize() - 1) {
            selectedIndex++;
            if (selectedIndex > highBound) {
                displayStartIndex++;
            }
            return Redraw;
        }
        return NoOp;
    case ' ':
        inventory.UseItem(selectedIndex);
        size = inventory.GetSize();
        if (selectedIndex >= size) {
            selectedIndex = size - 1;
        } 
        return Redraw;
    case 'q':
        return Exit;
    default:
        return NoOp;
    }
}

int InventoryHandler::selected() const { return selectedIndex; }
int InventoryHandler::start() const { return displayStartIndex; }