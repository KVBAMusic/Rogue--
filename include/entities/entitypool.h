#pragma once

#include "entities/testenemy.h"
#include "entities/itementity.h"
#include <vector>

template <class T>
class WeightedEntity {
    static_assert(std::is_base_of<Entity, T>::value, "Given type is not a valid entity");

public:
    T *entity;
    uint weight;
};


template <class T>
class EntityPool {
    static_assert(std::is_base_of<Entity, T>::value, "Given type is not a valid entity");

private:
    std::vector<WeightedEntity<T>> pool;
    uint totalWeight;

public:
    EntityPool();
    ~EntityPool();
    void AddEntity(T *e, uint weight);
    T *SpawnRandom() const;
};
