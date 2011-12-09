#ifndef MOVEMENT_GENERATOR_H
#define MOVEMENT_GENERATOR_H

#include <d3dx9.h>
#include <list>

struct sMoveCommand
{
    float m_MoveX, m_MoveY;
    UINT m_MoveTime;
    bool m_bWithCollission;

    sMoveCommand()
    {
        m_MoveX             = 0;
        m_MoveY             = 0;
        m_MoveTime          = 0;
        m_bWithCollission   = true;
    }
};

typedef std::list<sMoveCommand*> MoveCommandList;

class MovementGenerator
{
public:
    MovementGenerator(D3DXVECTOR2 *pPosition);
    ~MovementGenerator(void);

    D3DXVECTOR2 Move2DWithoutCollision(int x, int y, UINT uiMSECTime);
    /*D3DXVECTOR2 Move2D(int x, int y, UINT uiMSECTime);*/
    D3DXVECTOR3 UpdateMovement(const UINT uiCurTime, const UINT uiDiff);
    void ClearMovement();
    void RemoveMovementCommand(sMoveCommand* pCommand);
    inline bool IsMoveCommandListEmpty() { return m_lMoveCommands.empty(); }

protected:
    D3DXVECTOR2 *m_pPosition;

private:
    void Move(int x, int y);

    D3DXVECTOR2 m_v2CurMovement;
    MoveCommandList m_lMoveCommands;
};

#endif;