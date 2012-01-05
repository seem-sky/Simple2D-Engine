#ifndef SCRIPT_POINT_H
#define SCRIPT_POINT_H

#include <windows.h>
#include <string>
#include "Unit.h"
#include "Point.h"

enum ScriptPointType
{
    SCRIPT_POINT_NONE,
    SCRIPT_POINT_TELEPORT,
};

static ScriptPointType WrapToScriptPointType(UINT uiType)
{
    switch(uiType)
    {
    case 1:
        return SCRIPT_POINT_TELEPORT;
    case 0:
    default:
        return SCRIPT_POINT_NONE;
    }
}

struct ScriptPointPrototype
{
    Point<UINT> m_Position;
    Point<UINT> m_Size;

    ScriptPointType m_Type;

    ScriptPointPrototype() : m_Position(0,0), m_Size(0,0), m_Type(SCRIPT_POINT_NONE)
    {
        memset(&Type, 0, sizeof(Type));
    }

    union
    {
        struct ScriptPointTeleport
        {
            UINT m_uiTelePosX;
            UINT m_uiTelePosY;
            UINT m_uiMapID;
        }Teleport;
    }Type;
};

class ScriptPoint
{
public:
    ScriptPoint(void);
    ~ScriptPoint(void);

    void Update(const UINT uiDiff, const UINT uiTime);

    inline void SetPositionX(UINT x) { m_ScriptPointInfo.m_Position.x = x; }
    inline void SetPositionY(UINT y) { m_ScriptPointInfo.m_Position.y = y; }
    inline Point<UINT> GetPosition() { return m_ScriptPointInfo.m_Position; }
    inline void SetSizeX(UINT x) { m_ScriptPointInfo.m_Size.x = x; }
    inline void SetSizeY(UINT y) { m_ScriptPointInfo.m_Size.y = y; }
    inline Point<UINT> GetSize() { return m_ScriptPointInfo.m_Size; }
    inline void SetType(ScriptPointType type) { m_ScriptPointInfo.m_Type = type; }

    void SetScriptPointInfo(ScriptPointPrototype* pInfo);

    void DoAction(Unit *pWho);

private:
    virtual void UpdatePoint(const UINT uiDiff, const UINT uiTime) { }

    ScriptPointPrototype m_ScriptPointInfo;
};
#endif;