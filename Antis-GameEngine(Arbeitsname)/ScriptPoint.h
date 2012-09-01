#ifndef SCRIPT_POINT_H
#define SCRIPT_POINT_H

#include <windows.h>
#include <string>
#include "Unit.h"
#include "Point.h"
#include "WrapperFunctions.h"

struct ScriptPointPrototype
{
    Point<uint32> m_Position;
    Point<uint32> m_Size;

    ScriptPointType m_Type;

    ScriptPointPrototype() : m_Position(0,0), m_Size(0,0), m_Type(SCRIPT_POINT_NONE)
    {
        memset(&Type, 0, sizeof(Type));
    }

    union
    {
        struct ScriptPointTeleport
        {
            uint32 m_uiTelePosX;
            uint32 m_uiTelePosY;
            uint32 m_uiMapID;
        }Teleport;
    }Type;
};

class ScriptPoint
{
public:
    ScriptPoint(void);
    ~ScriptPoint(void);

    void Update(const uint32 uiDiff, const uint32 uiTime);

    inline void SetPositionX(uint32 x) { m_ScriptPointInfo.m_Position.x = x; }
    inline void SetPositionY(uint32 y) { m_ScriptPointInfo.m_Position.y = y; }
    inline Point<uint32> GetPosition() { return m_ScriptPointInfo.m_Position; }
    inline void SetSizeX(uint32 x) { m_ScriptPointInfo.m_Size.x = x; }
    inline void SetSizeY(uint32 y) { m_ScriptPointInfo.m_Size.y = y; }
    inline Point<uint32> GetSize() { return m_ScriptPointInfo.m_Size; }
    inline void SetType(ScriptPointType type) { m_ScriptPointInfo.m_Type = type; }

    void SetScriptPointInfo(ScriptPointPrototype* pInfo);

    void DoAction(Unit *pWho);

private:
    virtual void UpdatePoint(const uint32 uiDiff, const uint32 uiTime) { }

    ScriptPointPrototype m_ScriptPointInfo;
};
#endif;