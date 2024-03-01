#pragma once
#include "entities/attackable.h"
#include "items/inventory.h"

class Player : public Attackable {
private:
    ActionCallback openInventory;

    Attackable *target;
    Inventory inventory;

public:
    Player(Pos pos = {0}, EntityStats stats = {0});
    Player(const Player &other);
    ~Player();

    EntityAction Tick() override;
    EntityAction Move(int dx, int dy);
    void TakeDamage(const uint damage) override;

    const Attackable *GetTarget() const;
    const EntityStats GetStats() const override;
    Inventory &GetInventory();

    void SetTarget(Attackable *target);
    void SetInventoryCallback(ActionCallback cb);
    void SetEntityStats(EntityStats stats) override;
};