#include "items/wearable.h"

Wearable::Wearable(const char *name, const bool isWeapon, StatModifiers stats) :
  Item(name),
  modifiers(stats) {
    this->isWeapon = isWeapon;
}

Wearable::Wearable(const Wearable &other) : 
  Item(other),
  modifiers(other.modifiers) {
    isWeapon = other.isWeapon;
}

Wearable::~Wearable() {}

bool Wearable::IsWeapon() const { return isWeapon; }

const StatModifiers Wearable::GetModifiers() const { return modifiers; }