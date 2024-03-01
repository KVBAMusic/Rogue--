#include "entities/entity.h"
#include "level.h"
#include <pdcurses.h>
#include <cstring>

Entity::Entity(Pos position, const int health, const int strength, const int defence) :
  _pos(position),
  _stats({health, strength, defence}),
  dispData({0}) {
    level = nullptr;
    name = new char[1];
}

Entity::Entity(const Entity &other) :
  _pos(other._pos),
  _stats(other._stats),
  dispData(other.dispData) {
    level = nullptr;
    name = new char[strlen(other.name) + 1];
    strcpy_s(name, strlen(other.name) + 1, other.name);
}

Entity::Entity(Pos position, EntityStats stats) :
  _pos(position),
  _stats(stats),
  dispData({0}) {
    level = nullptr;
    name = new char[1];
}

const EntityStats Entity::GetStats() const { return _stats; }
Pos Entity::GetPosition() const { return _pos; }
const EntityDisplayData Entity::GetDisplayData() const { return dispData; }
const bool Entity::Dead() const { return _stats.health <= 0; }

void Entity::SetParentLevel(Level *level) { this->level = level; }
void Entity::SetDisplayData(EntityDisplayData data) { 
    dispData = data;
}
void Entity::SetEntityStats(EntityStats stats) { _stats = stats; }

void Entity::SetPosition(Pos pos) { _pos = pos; }

EntityAction Entity::Tick() {
    if (Dead() || moveDelay > 0) {
        moveDelay--;
        return Fail;
    }
    moveDelay = _stats.moveDelay;
    return Ok;
}

EntityAction Entity::Move(int dx, int dy) {
    return Ok;
}
