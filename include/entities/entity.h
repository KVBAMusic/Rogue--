#pragma once
#include "types.h"

class Level;

struct EntityMoveData;
typedef std::function<EntityMoveData(int, int)> EntityMoveCallback;

class Entity {
protected:
    Level *level;

    Pos _pos;
    EntityStats _stats;
    EntityDisplayData dispData;

    int moveDelay = 0;

public:
    char *name;

    Entity(Pos position = {0, 0}, const int health = 10, const int strength = 0, const int defence = 0);
    Entity(Pos position, EntityStats stats);
    Entity(const Entity &other);
    virtual ~Entity() { delete[] name; }

    virtual const EntityStats GetStats() const;
    Pos GetPosition() const;
    const EntityDisplayData GetDisplayData() const;
    const bool Dead() const;

    void SetParentLevel(Level *level);
    void SetDisplayData(EntityDisplayData data);
    virtual void SetEntityStats(EntityStats stats);

    void SetPosition(Pos pos);

    virtual EntityAction Tick();
    virtual EntityAction Move(int dx, int dy);
};

struct EntityMoveData {
    int targetTile;
    Entity *targetEntity;
};