#ifndef AI_MANAGER_H
#define AI_MANAGER_H

#include "Singleton.h"
#include <map>

class MapAI;

struct AIScript
{
    AIScript() : m_pMapScript(NULL) {};

    std::string m_sScriptName;
    MapAI* (*m_pMapScript)();

    void RegisterSelf();
};

typedef std::map<std::string, AIScript*> ScriptList;

class AI_Manager : public TSingleton<AI_Manager>
{
public:
    AI_Manager(void);
    ~AI_Manager(void);

    void LoadScripts();

    void AddMapScript(AIScript* p_pScript);
private:

    ScriptList m_ScriptList;
};
#endif