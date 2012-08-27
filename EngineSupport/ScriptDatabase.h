#ifndef SCRIPT_DATABASE_H
#define SCRIPT_DATABASE_H

#include "Singleton.h"

namespace EVENT_SCRIPT
{
    enum SCRIPT_COMMAND
    {
        CHANGE_VARIABLE,
    };

    struct EventCommand
    {
        EventCommand()
        {
            memset(&m_Command, 0, sizeof(m_Command));
        }

        SCRIPT_COMMAND m_CommandType;

        union
        {
            struct CommandChangeVariable
            {
                
            } m_ChangeVariable;
        } m_Command;
    };

    class ScriptDatabase : TSingleton<ScriptDatabase>
    {
    public:
        ScriptDatabase(void);
        ~ScriptDatabase(void);
    };
}
#endif