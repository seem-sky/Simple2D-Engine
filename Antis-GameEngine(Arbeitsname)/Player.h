#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"

// key action ID
enum
{
    ACTION_NONE,
    ACTION_MOVE_UP,
    ACTION_MOVE_DOWN,
    ACTION_MOVE_RIGHT,
    ACTION_MOVE_LEFT,
    ACTION_ESCAPE,
    ACTION_ENTER,
    ACTION_PAUSE_GAME,
};

enum MOVE_BUFFER
{
    MOVE_BUFFER_NONE    = 0x00,
    MOVE_BUFFER_UP      = 0x01,
    MOVE_BUFFER_DOWN    = 0x02,
    MOVE_BUFFER_RIGHT   = 0x04,
    MOVE_BUFFER_LEFT    = 0x08,
};

struct PlayerKeyAction
{
    UINT m_uiKey;
    bool m_bShouldPressed;
    UINT m_uiActionID;

    ULONGLONG m_uiLastTimeActive;

    PlayerKeyAction(void)
    {
        m_uiKey             = 0;
        m_bShouldPressed    = true;
        m_uiActionID        = 0;
        m_uiLastTimeActive  = 0;
    }
};

typedef std::list<PlayerKeyAction*> KeyList;

class Player
{
public:
    Player(void);
    ~Player(void);

    // read out keystates and other player related stuff
    void UpdatePlayer(const ULONGLONG CurTime, const UINT CurElapsedTime);

    // changes controle of an unit
    void SetControledUnit(Unit *pWho);

    // add a key action
    void AddKeyAction(UINT uiKey, UINT actionID, bool press = true);

    // move player object
    void MovePlayer(int XMove, int YMove, UINT uiMoveMSec);

protected:
    std::string m_sLogLocationName;

private:
    Unit *m_pControledUnit;

    KeyList m_KeyList;

    // move
    UINT m_uiMoveBuffer;
    MOVE_BUFFER m_uiLockBuffer;
    inline bool HasMoveBuffer(MOVE_BUFFER flag)
    {
        return (m_uiMoveBuffer & flag) != 0;
    }

    inline void SetMoveBuffer(MOVE_BUFFER flag)
    {
        m_uiMoveBuffer = m_uiMoveBuffer | flag;
    }

    void MovePlayerByBuffer();
    void DoActionForKey(PlayerKeyAction *action, const ULONGLONG CurTime);
};
#endif;