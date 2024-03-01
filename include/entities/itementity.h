#pragma once
#include "entities/entity.h"
#include "items/item.h"

class ItemEntity : public Entity {
private:
    Item _item;
public:
    ItemEntity(const Item &item);
    ItemEntity(const ItemEntity &other);
    ~ItemEntity();

    Item GetItem() const;
};