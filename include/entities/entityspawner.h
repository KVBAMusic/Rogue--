#pragma once
#include "level.h"
#include <cstring>

template <class T>
class EntityFactory {
    
static_assert(std::is_base_of<Entity, T>::value, "Given type is not a valid entity");

private:
    T *entity;

public:
    EntityFactory<T>();
    EntityFactory<T>(const T &src);

    EntityFactory<T> &SetDisplayData(char dispChar, short fg, short bg);
    EntityFactory<T> &SpawnAtLevelStart(Level &level);
    EntityFactory<T> &SetName(const char *name);
    EntityFactory<T> &SpawnAwayFromPlayer(Level &level, Player *player);
    EntityFactory<T> &SetEntityStats(EntityStats stats);
    T *Return();
};