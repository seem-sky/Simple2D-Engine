#ifndef UNIT_AI_H
#define UNIT_AI_H

#include "ObjectAI.h"

class Unit;

class UnitAI : public ObjectAI
{
public:
    UnitAI();
    virtual ~UnitAI();

    virtual void SetOwner(Unit* pOwner);

    // movement
    void OnPathComplete();
};
#endif;