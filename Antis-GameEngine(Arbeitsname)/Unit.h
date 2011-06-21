#ifndef UNIT_H
#define UNIT_H

#include "WorldObject.h"
#include "MovementGenerator.h"

class Unit : public WorldObject
{
public:
    Unit(void);
    virtual ~Unit(void);
    void Update(const UINT uiCurTime, const UINT uiDiff);

protected:
    MovementGenerator *m_pMovement;

private:
    UINT m_uiTimer;
};

#endif;