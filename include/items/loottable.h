#pragma once

#include "items/consumable.h"

struct LootTable {
    uint totalWeight;
    uint chanceOfFailure;
    std::vector<Item *> items;
    std::vector<uint> weights;

    LootTable();
};

class LootTableFactory {
private:
    LootTable *lt;
public:
    LootTableFactory();
    LootTableFactory &AddItem(Item &item, const uint weight);
    LootTableFactory &ChanceOfFailure(const uint weight);
    LootTable *Return();
};