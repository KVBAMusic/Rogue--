#include "entities/attackable.h"
#include <cstdlib>

Attackable::Attackable(Pos position, EntityStats stats) : Entity(position, stats) {
    maxHealth = stats.health;
    lootTable = nullptr;
}

Attackable::Attackable(const Attackable &other) : Entity(other) {
    maxHealth = other._stats.health;
    lootTable = other.lootTable;
}

void Attackable::TakeDamage(const uint damage) {
    if (damage <= _stats.defence) {
        return;
    }
    _stats.health -= damage - _stats.defence;
}

EntityAction Attackable::Move(int dx, int dy) {
    return Entity::Move(dx, dy);
}

EntityAction Attackable::Tick() {
    return Entity::Tick();
}

const uint Attackable::GetMaxHealth() const { return maxHealth; };

Item *Attackable::GetRandomItem() const {
    int r = rand() % lootTable->totalWeight;
    if (r < lootTable->chanceOfFailure) {
        return nullptr;
    }
    r -= lootTable->chanceOfFailure;
    int i = 0;
    while (r > 0) {
        r -= lootTable->weights[i];
        if (i < lootTable->weights.size() - 1) i++;
    }
    return lootTable->items[i];
}

void Attackable::SetLootTable(LootTable *table) { lootTable = table; }