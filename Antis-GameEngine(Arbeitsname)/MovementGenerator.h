#ifndef MOVEMENT_GENERATOR_H
#define MOVEMENT_GENERATOR_H

#include <list>
#include "Point.h"
#include "Pathfinder.h"
#include "WrapperFunctions.h"

struct sMoveCommand
{
    int m_MoveX, m_MoveY;
    UINT m_MoveTime;
    bool m_bWithCollission;
    DIRECTION m_Dir;
    bool m_bPathfinderMove;

    sMoveCommand() : m_MoveX(0), m_MoveY(0), m_MoveTime(0), m_bWithCollission(true), m_Dir(DIRECTION_NONE), m_bPathfinderMove(false) { }
};

typedef std::list<sMoveCommand*> MoveCommandList;

class Unit;

enum Collission_Type
{
    COLLISSION_NONE,
    COLLISSION_TILE,
    COLLISSION_OBJECT,
};

class MovementGenerator
{
public:
    MovementGenerator(Unit *pObj);
    ~MovementGenerator(void);

    void Move2D(int x, int y, UINT uiMSECTime, DIRECTION dir, bool Collission = true);
    void Move2DRandom(UINT uiRange, bool Collission = true);
    void MovePointByPathfinder(Point<UINT> uiPoint);

    bool UpdateMovement(const ULONGLONG uiCurTime, const UINT uiDiff);
    void ClearMovement();
    void RemoveMovementCommand(sMoveCommand* pCommand);
    inline bool IsMoveCommandListEmpty() { return m_lMoveCommands.empty(); }

    Collission_Type CheckMovement(Point<int> oldPos, Point<int> newPos, Point<int> &result);
    void CheckScriptPoint();

private:
    Point<float> m_fCurMovement;
    UINT m_uiCurMoveTime;
    MoveCommandList m_lMoveCommands;
    Unit *m_pObj;
    Pathfinder m_PathObj;
    bool m_bNewCommand;
};

#endif;