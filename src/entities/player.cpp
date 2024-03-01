#include "entities/player.h"
#include "level.h"
#include "gamelogs.h"
#include <string.h>
#include <pdcurses.h>

Player::Player(Pos pos, EntityStats stats) : 
  Attackable(pos, stats),
  inventory(Inventory()),
  openInventory([](){}) {
    target = nullptr;
    inventory.Clear();
    inventory.SetUseItemCallback([this](const StatModifiers &stats) { 
        maxHealth += stats.maxHealth;
        _stats.health += stats.health;
        if (_stats.health > maxHealth) {
            _stats.health = maxHealth;
        }
        _stats.defence += stats.defence;
        _stats.strength += stats.strength;
    });
}

Player::Player(const Player &other) : 
  Attackable(other),
  inventory(other.inventory),
  openInventory(other.openInventory) {
    target = nullptr;
    inventory.SetUseItemCallback([this](const StatModifiers &stats) { 
        maxHealth += stats.maxHealth;
        _stats.health += stats.health;
        if (_stats.health > maxHealth) {
            _stats.health = maxHealth;
        }
        _stats.defence += stats.defence;
        _stats.strength += stats.strength;
    });
}

Player::~Player() {
    
}

EntityAction Player::Tick() {
    EntityAction result = Attackable::Tick();
    if (result != Ok) {
        return result;
    }
    char c;
    while (1) {
        switch (c = getch())
        {
        case 'w':
            return Move(0, -1);
        case 's':
            return Move(0, 1);
        case 'a':
            return Move(-1, 0);
        case 'd':
            return Move(1, 0);
        case 'e':
            openInventory();
            moveDelay = 0;
            return NoMove;
        case 'p':
            return Quit;
        default:
            return NoMove;
        }
    }
}

EntityAction Player::Move(int dx, int dy) {
    int tileAhead = level->GetTileAt(_pos.x + dx, _pos.y + dy);
    Entity *entityAhead = level->GetEntityAtPos({_pos.x + dx, _pos.y + dy});
    target = dynamic_cast<Attackable *>(entityAhead);

    if (target != nullptr) {
        target->TakeDamage(GetStats().strength);
        if (target->Dead()) {
            Item * item = target->GetRandomItem();
            if (item != nullptr) {
                Logger::instance().LogMessage(1);
                inventory.AddItem(*item);
            }
            target = nullptr;
        }
        return Ok;
    }
    if (tileAhead == TILE_WALL || tileAhead == TILE_EMPTY) {
        moveDelay = 0;
        return NoMove;
    }
    if (tileAhead == TILE_END) {
        return NextLevel;
    }
    if (tileAhead == TILE_START) {
        return PreviousLevel;
    }
    _pos.Move(dx, dy);
    return Ok;
}

void Player::TakeDamage(const uint damage) {
    uint defence = GetStats().defence;
    if (damage <= defence) {
        return;
    }
    _stats.health -= damage - defence;
}

const Attackable *Player::GetTarget() const { return target; }
Inventory &Player::GetInventory() { return inventory; }
const EntityStats Player::GetStats() const { return _stats.ApplyModifiers(inventory.GetWornModifiers()); }

void Player::SetTarget(Attackable *target) { this->target = target; }
void Player::SetInventoryCallback(ActionCallback cb) {
    openInventory = cb;
}
void Player::SetEntityStats(EntityStats stats) {
    this->_stats = stats;
    maxHealth = stats.health;
}