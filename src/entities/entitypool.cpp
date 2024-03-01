#include "entities/entitypool.h"
#include <cstdlib>

template <class T>
EntityPool<T>::EntityPool() : pool(std::vector<WeightedEntity<T>>()) {
    totalWeight = 0;
}

template <class T>
EntityPool<T>::~EntityPool() {
    for (WeightedEntity<T> e : pool) {
        delete e.entity;
    }
    pool.clear();
}

template <class T>
void EntityPool<T>::AddEntity(T *e, uint weight) {
    pool.push_back(WeightedEntity<T>{e, weight});
    totalWeight += weight;
}

template <class T>
T *EntityPool<T>::SpawnRandom() const {
    int w = rand() % totalWeight; {
        for (const WeightedEntity<T> e : pool) {
            w -= e.weight;
            if (w < 0) {
                return e.entity;
            }
        }
    }
    return nullptr;
}

template class EntityPool<BaseEnemy>;
template class EntityPool<ItemEntity>;