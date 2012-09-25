#ifndef SCRIPT_DATABASE_H
#define SCRIPT_DATABASE_H

#include "Singleton.h"
#include "EventScriptCommand.h"

namespace EVENT_SCRIPT
{
    class ScriptDatabase : TSingleton<ScriptDatabase>
    {
    public:
        ScriptDatabase(void);
        ~ScriptDatabase(void);
    };
}
#endif