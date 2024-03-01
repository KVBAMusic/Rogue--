#pragma once
#include "entities/entity.h"
#include "items/loottable.h"

class Attackable : public Entity {
protected:
    uint maxHealth;
    LootTable *lootTable;

public:
    Attackable(Pos position = {0}, EntityStats stats = {0});
    Attackable(const Attackable &other);
    virtual ~Attackable() {}
    
    virtual void TakeDamage(const uint damage);
    virtual EntityAction Move(int dx, int dy) override;
    virtual EntityAction Tick() override;
    
    const uint GetMaxHealth() const;
    Item *GetRandomItem() const;

    void SetLootTable(LootTable *table);
};