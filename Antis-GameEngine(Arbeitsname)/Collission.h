#ifndef COLLISSION_H
#define COLLISSION_H

#include "Unit.h"

class Collission
{
public:
    static bool CheckTileCollission(Unit* pWho, Point<int> oldPos, Point<int> newPos, Point<int> &result);
    static bool CheckObjectCollission(Unit* pWho, Point<int> oldPos, Point<int> newPos, Point<int> &result);
};
#endif