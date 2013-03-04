#include "ScriptPoint.h"
#include "Map.h"
#include "Game.h"

ScriptPoint::ScriptPoint()
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

        if (Map *pMap = pWho->GetMap())
        {
            // teleport on same map
            if (pMap->GetMapInfo()->m_uiID == m_ScriptPointInfo.Type.Teleport.m_uiMapID)
            {
                pWho->ChangePosition(m_ScriptPointInfo.Type.Teleport.m_uiTelePosX, m_ScriptPointInfo.Type.Teleport.m_uiTelePosY);
                pMap->ChangePosition(Point<int>(m_ScriptPointInfo.Type.Teleport.m_uiTelePosX*(-1), m_ScriptPointInfo.Type.Teleport.m_uiTelePosY*(-1)));
            }
            // teleport on other map
            else
            {
                if (CGame *t_pGame = CGame::Get())
                {
                    GameEvent t_Event;
                    t_Event.m_EventType = EVENT_MAP_CHANGE;
                    t_Event.EventType.MapChange.m_uiMapID = m_ScriptPointInfo.Type.Teleport.m_uiMapID;
                    t_Event.EventType.MapChange.m_uiNewPlayerPosX = m_ScriptPointInfo.Type.Teleport.m_uiTelePosX;
                    t_Event.EventType.MapChange.m_uiNewPlayerPosY = m_ScriptPointInfo.Type.Teleport.m_uiTelePosY;
                    t_pGame->AddGameEvent(t_Event);
                }
            }
        }
        break;
    default:
        break;
    }
}