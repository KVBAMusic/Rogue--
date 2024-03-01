#include "entities/itementity.h"
#include <pdcurses.h>

ItemEntity::ItemEntity(const Item &item) : _item(item) {
    dispData = EntityDisplayData{'*', COLOR_YELLOW, COLOR_BLACK, };
}

ItemEntity::ItemEntity(const ItemEntity &other) : _item(other._item) {}

ItemEntity::~ItemEntity() {}

Item ItemEntity::GetItem() const { return _item; }