#include "items/loottable.h"

LootTable::LootTable() :
  items(std::vector<Item *>()),
  weights(std::vector<uint>()) {}

LootTableFactory::LootTableFactory() {
    lt = new LootTable;
    lt->totalWeight = 0;
}

LootTableFactory &LootTableFactory::AddItem(Item &item, const uint weight) {
    lt->items.push_back(&item);
    lt->weights.push_back(weight);
    lt->totalWeight += weight;
    return *this;
}

LootTableFactory &LootTableFactory::ChanceOfFailure(const uint weight) {
    lt->chanceOfFailure = weight;
    lt->totalWeight += weight;
    return *this;
}

LootTable *LootTableFactory::Return() { return lt; }