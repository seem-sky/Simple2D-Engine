#ifndef MOVEMENT_GENERATOR_H
#define MOVEMENT_GENERATOR_H

#include <d3dx9.h>
#include <list>
#include "Point.h"

struct sMoveCommand
{
    float m_MoveX, m_MoveY;
    UINT m_MoveTime;
    bool m_bWithCollission;

    sMoveCommand() : m_MoveX(0), m_MoveY(0), m_MoveTime(0), m_bWithCollission(true){ }
};

typedef std::list<sMoveCommand*> MoveCommandList;

class Unit;

class MovementGenerator
{
public:
    MovementGenerator(Unit *pObj);
    ~MovementGenerator(void);

    void Move2D(int x, int y, UINT uiMSECTime, bool Collission = true);
    void Move2DRandom(UINT uiRange, bool Collission = true);
    void UpdateMovement(const ULONGLONG uiCurTime, const UINT uiDiff);
    void ClearMovement();
    void RemoveMovementCommand(sMoveCommand* pCommand);
    inline bool IsMoveCommandListEmpty() { return m_lMoveCommands.empty(); }

    bool CheckMovement(Point<int> oldPos, Point<int> newPos, Point<int> &result);
    void CheckScriptPoint();

private:
    D3DXVECTOR2 m_v2CurMovement;
    MoveCommandList m_lMoveCommands;
    Unit *m_pObj;
};

#endif;