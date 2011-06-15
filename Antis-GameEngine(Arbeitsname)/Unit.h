#ifndef UNIT_H
#define UNIT_H

#include "WorldObject.h"
#include "MovementGenerator.h"

class Unit : public WorldObject
{
public:
    Unit(void);
    virtual ~Unit(void);

protected:
    void Update(const UINT uiCurTime, const UINT uiDiff);
    MovementGenerator *m_pMovement;
};

#endif;