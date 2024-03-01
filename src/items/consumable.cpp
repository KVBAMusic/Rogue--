#include "items/consumable.h"

Consumable::Consumable(const char *name, StatModifiers stats) :
  Item(name), 
  modifiers(stats) {}

Consumable::Consumable(const Consumable &other) : 
  Item(other),
  modifiers(other.modifiers) {}

Consumable::~Consumable() {}

const StatModifiers Consumable::GetModifiers() const { return modifiers; }