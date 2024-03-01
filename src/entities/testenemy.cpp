#include "entities/testenemy.h"
#include "level.h"

BaseEnemy::BaseEnemy(Pos position, EntityStats stats) : Attackable(position, stats) {}

BaseEnemy::BaseEnemy(const BaseEnemy &other) : Attackable(other) {}

EntityAction BaseEnemy::Tick() {
    EntityAction result = Attackable::Tick();
    if (result != Ok) {
        return result;
    }
    
    Player *player = nullptr;
    for (Entity *e : level->GetEntities()) {
        player = dynamic_cast<Player *>(e);
        if (player != nullptr) {
            if (_pos.DistanceTo(player->GetPosition()) > _stats.viewRadius) {
                return Ok;
            }
            Pos dir = GetDirectionToPlayer(player);
            return Move(dir.x, dir.y);
        }
    }

    return Ok;
}

EntityAction BaseEnemy::Move(int dx, int dy) {
    int tileAhead = level->GetTileAt(_pos.x + dx, _pos.y + dy);
    Entity *entityAhead = level->GetEntityAtPos({_pos.x + dx, _pos.y + dy});
    Attackable *anyEntityAhead = dynamic_cast<Attackable *>(entityAhead);
    Player *player = dynamic_cast<Player *>(entityAhead);
    if (tileAhead == TILE_EMPTY || tileAhead == TILE_WALL) {
        return Fail;
    }
    if (player != nullptr) {
        player->TakeDamage(_stats.strength);
        return Ok;
    }
    if (anyEntityAhead != nullptr) {
        return Fail;
    }
    _pos.Move(dx, dy);
    return Ok;
}

Pos BaseEnemy::GetDirectionToPlayer(Player *player) {
    Pos diff = player->GetPosition() - _pos;
    Pos output = {0, 0};
    output.x = diff.x < 0 ? -1 : 
                diff.x > 0 ? 1 : 0;
    output.y = diff.y < 0 ? -1 :
                diff.y > 0 ? 1 : 0;
    if (abs(diff.y) > abs(diff.x)) {
        output.x = 0;
    }
    return output;
}