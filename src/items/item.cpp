#include "items/item.h"
#include <string.h>

Item::Item(const char *name) {
    int len = strlen(name) + 1;
    this->name = new char[len];
    strcpy_s(this->name, len, name);
}

Item::Item(const Item &other) {
    int len = strlen(other.name) + 1;
    this->name = new char[len];
    strcpy_s(this->name, len, other.name);
}

Item::~Item() {
    delete[] name;
}

const char *Item::GetName() const { return name; }