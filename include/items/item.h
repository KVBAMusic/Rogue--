#pragma once
#include "types.h"

class Item {
protected:
    char *name;
public:
    Item(const char *name = "");
    Item(const Item &other);
    virtual ~Item();

    const char *GetName() const;
};
