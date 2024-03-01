#pragma once
#include "items/item.h"

class Consumable : public Item {
private:
    StatModifiers modifiers;

public:
    Consumable(const char *name = "", StatModifiers stats = {0});
    Consumable(const Consumable &other);
    ~Consumable() override;

    const StatModifiers GetModifiers() const;
};