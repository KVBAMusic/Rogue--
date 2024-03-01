#pragma once
#include "entities/player.h"

class BaseEnemy : public Attackable {
private:
    Pos GetDirectionToPlayer(Player *player);

public:
    BaseEnemy(Pos position = {0}, EntityStats stats = {0});
    BaseEnemy(const BaseEnemy &other);
    ~BaseEnemy() {}

    EntityAction Move(int dx, int dy) override;
    EntityAction Tick() override;
};