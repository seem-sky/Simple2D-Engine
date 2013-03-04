#include "MapAI.h"
MapAI* GetMapScript()
{
    return new MapAI();
}

void AddScript_MapAI()
{
    AIScript *t_pScript = new AIScript();
    t_pScript->m_pMapScript = &GetMapScript;
}