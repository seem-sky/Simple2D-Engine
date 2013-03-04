#ifndef EVENT_SCRIPT_HOLDER_H
#define EVENT_SCRIPT_HOLDER_H

#include "EventScriptCommand.h"
#include "VariableHolder.h"

namespace EVENT_SCRIPT
{
    class EventScript
    {
    public:
        inline VariableHolder* GetLocalVariableHolder() { return &m_LocalVariables; }
        inline const VariableHolder* GetLocalVariableHolder() const { return &m_LocalVariables; }

        void InsertCommand(uint32 p_uiIndex, const EventScriptCommandPtr &p_Command);
        bool GetCommand(UINT32 p_uiIndex, EventScriptCommandPtr &p_pResult) const;
        void DeleteCommand(uint32 p_uiIndex);
        void DeleteCommand(const EventScriptCommandPtr &p_Command);
        void DeleteCommandsOfType(ScriptCommandType p_Type);
        uint32 GetCommandCount() const { return m_pCommands.size(); }

        std::string GetName() const { return m_sName; }
        void SetName(const std::string &p_sName) { m_sName = p_sName; }

        bool GetXMLData(XML::XML_WriteData &p_Data) const;
        bool LoadDataFromXML(const XML::XML_ReadData &p_Data);

    private:
        std::string m_sName;
        EventScriptCommandPtrList m_pCommands;
        VariableHolder m_LocalVariables;
    };

    typedef std::vector<EventScript> EventScriptList;

    class EventScriptHolder
    {
    public:
        static bool GetNewCommand(ScriptCommandType p_Type, EventScriptCommandPtr &p_Result);
        EventScript* InsertEventScript(int p_uiIndex = -1);
        EventScript* GetEventScript(uint32 p_uiIndex);
        void MoveEventScript(uint32 p_uiOldIndex, uint32 p_uiNewIndex);
        void MoveEventScript(const EventScript *p_pEventScript, uint32 p_uiNewIndex);
        void DeleteEventScript(uint32 p_uiIndex);
        void DeleteEventScript(const EventScript *p_pEventScript);
        uint32 GetEventScriptCount() { return m_Scripts.size(); }
        void DeleteCommandsOfTypeInAllScripts(ScriptCommandType p_Type);

        void SetScriptName(const std::string &p_sScriptName, const uint32 &p_uiIndex);
        bool GetScriptName(const uint32 &p_uiIndex, std::string &p_sResult);

        bool GetXMLData(XML::XML_WriteData &p_Data) const;
        void LoadDataFromXML(const XML::XML_ReadData &p_Data);

    private:
        int32 GetEventScriptIndex(const EventScript *p_pEventScript);

        EventScriptList m_Scripts;
    };
}
#endif