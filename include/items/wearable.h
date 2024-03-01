#pragma once
#include "items/item.h"

class Wearable : public Item {
private:
    StatModifiers modifiers;
    bool isWeapon;

public:
    Wearable(const char *name = "", const bool isWeapon = true, StatModifiers stats = {0});
    Wearable(const Wearable &other);
    ~Wearable() override;

    bool IsWeapon() const;

    const StatModifiers GetModifiers() const;
};