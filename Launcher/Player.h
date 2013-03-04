#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>
#include <list>
#include "Global.h"

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
    uint32 m_uiKey;
    bool m_bShouldPressed;
    uint32 m_uiActionID;

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

class Unit;
class WorldObject;

class Player
{
public:
    Player(void);
    ~Player(void);

    // read out keystates and other player related stuff
    void UpdatePlayer(const ULONGLONG CurTime, const uint32 CurElapsedTime);

    // changes controle of an unit
    void SetControledUnit(Unit *pWho);

    // add a key action
    void AddKeyAction(uint32 uiKey, uint32 actionID, bool press = true);

    // move player object
    void MovePlayer(int XMove, int32 YMove, uint32 uiMoveMSec);

    void UseCollissionObject();

    bool CanUseObject(WorldObject* pWho);

    inline Unit* GetControledUnit() { return m_pControledUnit; }

protected:
    std::string m_sLogLocationName;

private:
    Unit *m_pControledUnit;

    KeyList m_KeyList;

    // move
    uint32 m_uiMoveBuffer;
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