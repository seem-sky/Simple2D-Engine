#include "AI_Manager.h"
#include "AI_Loader.h"

AI_Manager::AI_Manager(void)
{
}

AI_Manager::~AI_Manager(void)
{
    for (ScriptPage::iterator t_itr = m_ScriptList.begin(); t_itr != m_ScriptList.end(); ++t_itr)
        delete t_itr->second;
}

void AI_Manager::AddMapScript(AIScript *p_pScript)
{
    if (p_pScript)
        m_ScriptList.insert(std::make_pair(p_pScript->m_sScriptName, p_pScript));
}

void AI_Manager::LoadScripts()
{
    //AI_LOADER::LoadAI();
}

void AIScript::RegisterSelf()
{
    if (AI_Manager *t_pAIMgr = AI_Manager::Get())
        t_pAIMgr->AddMapScript(this);
}