#include "entities/entityspawner.h"

template <class T>
EntityFactory<T>::EntityFactory() {
    entity = new T();
}

template <class T>
EntityFactory<T>::EntityFactory(const T& src) {
    entity = new T(src);
}

template <class T>
EntityFactory<T> &EntityFactory<T>::SetDisplayData(char dispChar, short fg, short bg) {
    entity->SetDisplayData(EntityDisplayData{dispChar, fg, bg});
    return *this;
}

template <class T>
EntityFactory<T> &EntityFactory<T>::SpawnAtLevelStart(Level &level) {
    entity->SetPosition(level.GetStart());
    return *this;
}

template <class T>
EntityFactory<T> &EntityFactory<T>::SetName(const char *name) {
    uint len = strlen(name) + 1;
    delete[] entity->name;
    entity->name = new char[len];
    strcpy_s(entity->name, len, name);
    return *this;
}

template <class T>
EntityFactory<T> &EntityFactory<T>::SpawnAwayFromPlayer(Level &level, Player *player) {
    Pos target = {-1, -1};
    for (Pos p : level.GetSpawnerPositions()) {
        if (level.GetEntityAtPos(p) != nullptr) continue;
        if (p.DistanceTo(player->GetPosition()) > 15) continue;
        if (p.DistanceTo(player->GetPosition()) < 8) continue;
        target = p;
    break;
    }
    entity->SetPosition(target);
    return *this;
}

template <class T>
EntityFactory<T> &EntityFactory<T>::SetEntityStats(EntityStats stats) {
    entity->SetEntityStats(stats);
    T *newE = new T(*entity);
    delete entity;
    entity = newE;
    return *this;
}

template <class T>
T *EntityFactory<T>::Return() {
    return entity;
}
template class EntityFactory<Entity>;
template class EntityFactory<Player>;
template class EntityFactory<BaseEnemy>;