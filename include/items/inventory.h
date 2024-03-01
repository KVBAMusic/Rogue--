#pragma once

#include "items/consumable.h"
#include "items/wearable.h"

typedef std::function<void(const StatModifiers &)> UseItemCallback;

struct InvNode {
    Consumable *c;
    Wearable *w;
    uint count;
    bool equipped;
    InvNode *next;
    InvNode *prev;

    ~InvNode() {
        if (c != nullptr) delete c;
        if (w != nullptr) delete w;
    }

    const char *GetName() {
        return c == nullptr ? w->GetName() : c->GetName();
    }
};

class Inventory {
private:
    InvNode *head;
    uint size;
    UseItemCallback useItem;

    InvNode *armour;
    InvNode *weapon;

    InvNode *NewNode(const Consumable *srcC, const Wearable *srcW, uint count);
    void AddToList(InvNode *node);

public:
    Inventory();
    ~Inventory();

    void AddItem(const Item &item);
    void UseItem(int idx);
    void RemoveItem(int idx);
    void RemoveItem(InvNode *node);
    void Clear();
    bool IsEquipped(InvNode *item) const;

    InvNode *GetItems();
    const uint GetSize() const;
    InvNode *GetAtIdx(int idx);
    StatModifiers GetWornModifiers() const;

    void SetUseItemCallback(UseItemCallback cb);
};