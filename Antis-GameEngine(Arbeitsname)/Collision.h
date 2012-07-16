#ifndef COLLISION_H
#define COLLISION_H

#include "Unit.h"

class Collision
{
public:
    static bool CheckTileCollision(Unit* pWho, Point<int> oldPos, Point<int> newPos, Point<int> &result);
    static bool CheckObjectCollision(Unit* pWho, Point<int> oldPos, Point<int> newPos, Point<int> &result);
};
#endif