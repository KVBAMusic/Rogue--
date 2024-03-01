#pragma once
#include "constants.h"
#include <functional>
#include <ctime>
#include <cmath>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulong;

typedef std::function<void()> ActionCallback;

struct Pos {
    int x;
    int y;

    void Move(const int dx, const int dy) {
        x += dx;
        y += dy;
    }

    static Pos Random(int maxX, int maxY) {
        srand(time(0));
        return Pos{rand() % maxX, rand() % maxY};
    }

    float DistanceTo(const Pos& other) {
        int xDist = x - other.x;
        int yDist = y - other.y;
        return sqrt(xDist * xDist + yDist * yDist);
    }

    bool operator ==(const Pos &other) const {
        return this->x == other.x && this->y == other.y;
    }
    bool operator !=(const Pos &other) const {
        return this->x != other.x || this->y != other.y;
    }
    Pos operator -(const Pos &other) {
        return {x - other.x, y - other.y};
    }
};

enum EntityAction {
    Ok = 0,
    Fail = 1,
    NoMove = 2,
    Quit = 4,
    NextLevel = 8,
    PreviousLevel = 16,
};

struct StatModifiers {
    int health;
    int maxHealth;
    int strength;
    int defence;
    int hunger;
    int speed;

    void operator+=(const StatModifiers &other) {
        this->health += other.health;
        this->maxHealth += other.maxHealth;
        this->strength += other.strength;
        this->defence += other.defence;
        this->hunger += other.hunger;
        this->speed += other.speed;
    }
};

struct EntityStats {
    int health;
    int strength;
    int defence;
    int moveDelay;
    int viewRadius;

    EntityStats ApplyModifiers(const StatModifiers &other) const {
        return {
            this->health + other.health,
            this->strength + other.strength,
            this->defence + other.defence,
            this->moveDelay,
            this->viewRadius
        };
    }
};

struct EntityDisplayData {
    char dispChar;
    short fg;
    short bg;
};
