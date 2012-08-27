#include "ScriptDatabase.h"

namespace EVENT_SCRIPT
{
    ScriptDatabase::ScriptDatabase(void) : TSingleton()
    {
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "ScriptDatabase : ";
    }

    ScriptDatabase::~ScriptDatabase(void)
    {
    }
}