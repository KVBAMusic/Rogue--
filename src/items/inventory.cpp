#include "items/inventory.h"
#include <cstring>
#include <iostream>

Inventory::Inventory() :
  useItem([](const StatModifiers &m){}) {
    head = nullptr;
    size = 0;
    armour = nullptr;
    weapon = nullptr;
}

Inventory::~Inventory() {
    Clear();
}

void Inventory::AddItem(const Item &item) {
    const Consumable *c = dynamic_cast<const Consumable *>(&item);
    const Wearable *w = dynamic_cast<const Wearable *>(&item);
    if (c == nullptr) {
        AddToList(NewNode(nullptr, w, 1));
        return;
    }
    InvNode *curr = head;
    while (curr != nullptr) {
        if (strcmp(curr->GetName(), item.GetName()) == 0) {
            curr->count++;
            return;
        }
        curr = curr->next;
    }
    AddToList(NewNode(c, nullptr, 1));
}

void Inventory::UseItem(int idx) {
    InvNode *item = GetAtIdx(idx);
    if (item == nullptr) {
        return;
    }
    Consumable *c = item->c;
    if (c != nullptr) {
        useItem(c->GetModifiers());
        if (--item->count <= 0) {
            RemoveItem(item);
        }
        return;
    }

    Wearable *w = item->w;
    if (w != nullptr) {
        if (w->IsWeapon()) {
            if (weapon != nullptr) {
                weapon->equipped = false;
            }
            item->equipped = true;
            weapon = item;
            return;
        }
        if (armour != nullptr) {
            armour->equipped = false;
        }
        item->equipped = true;
        armour = item;
    }
}

void Inventory::RemoveItem(InvNode *node) {
    if (node == nullptr) {
        return;
    }
    if (head == node) {
        head = node->next;
    }

    if (node->next != nullptr) {
        node->next->prev = node->prev;
    }
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    }
    delete node;
    size--;
}

void Inventory::RemoveItem(int idx) {
    RemoveItem(GetAtIdx(idx));
}

void Inventory::Clear() {
    if (head == nullptr) {
        return;
    }
    InvNode *curr = head;
    while (curr->next != nullptr) {
        curr = curr->next;
        delete curr->prev;
    }
    delete curr;
    head = nullptr;
    size = 0;
    armour = nullptr;
    weapon = nullptr;
}

bool Inventory::IsEquipped(InvNode *item) const {
    return item->equipped;
}

InvNode *Inventory::GetItems() { return head; }

const uint Inventory::GetSize() const { return size; }

void Inventory::SetUseItemCallback(UseItemCallback cb) {
    useItem = cb;
}

InvNode *Inventory::NewNode(const Consumable *srcC, const Wearable *srcW, uint count) {
    InvNode *n = new InvNode;
    n->next = nullptr;
    n->prev = nullptr;
    n->c = srcC == nullptr ? nullptr : new Consumable(*srcC);
    n->w = srcW == nullptr ? nullptr : new Wearable(*srcW);
    n->count = count;
    n->equipped = false;
    return n;
}

void Inventory::AddToList(InvNode *node) {
    if (head == nullptr) {
        head = node;
        size++;
        return;
    }
    InvNode *curr = head;
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = node;
    node->prev = curr;
    size++;
}

InvNode *Inventory::GetAtIdx(int idx) {
    InvNode *curr = head;
    while (curr != nullptr && idx > 0) {
        curr = curr->next;
        idx--;
    }
    return curr;
}

StatModifiers Inventory::GetWornModifiers() const {
    StatModifiers output = {0};
    if (armour != nullptr) {
        output += armour->w->GetModifiers();
    }
    if (weapon != nullptr) {
        output += weapon->w->GetModifiers();
    }
    return output;
}