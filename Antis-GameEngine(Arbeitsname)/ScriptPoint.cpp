#include "ScriptPoint.h"
#include "Map.h"

ScriptPoint::ScriptPoint(void)
{
}

ScriptPoint::~ScriptPoint(void)
{
}

void ScriptPoint::SetScriptPointInfo(ScriptPointPrototype *pInfo)
{
    if (!pInfo)
        return;

    m_ScriptPointInfo = *pInfo;
}

void ScriptPoint::DoAction(Unit *pWho)
{
    switch(m_ScriptPointInfo.m_Type)
    {
    case SCRIPT_POINT_TELEPORT:
        if (!pWho)
            return;

        pWho->ChangePosition(m_ScriptPointInfo.Type.Teleport.m_uiTelePosX, m_ScriptPointInfo.Type.Teleport.m_uiTelePosY);
        if (Map *pMap = pWho->GetMap())
            pMap->ChangePosition(Point<int>(m_ScriptPointInfo.Type.Teleport.m_uiTelePosX*(-1), m_ScriptPointInfo.Type.Teleport.m_uiTelePosY*(-1)));
        break;
    default:
        break;
    }
}