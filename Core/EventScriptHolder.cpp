#include "EventScriptHolder.h"
#include <atlstr.h>

using namespace XML;

namespace EVENT_SCRIPT
{
    /*#####
    # EventScript
    #####*/
    void EventScript::InsertCommand(uint32 p_uiIndex, const EventScriptCommandPtr &p_Command)
    {
        EventScriptCommandPtrList::iterator t_Itr = m_pCommands.begin();
        t_Itr += p_uiIndex;
        m_pCommands.insert(t_Itr, p_Command);
    }

    bool EventScript::GetCommand(UINT32 p_uiIndex, EventScriptCommandPtr &p_pResult) const
    {
        if (p_uiIndex >= m_pCommands.size())
            return false;

        p_pResult = m_pCommands.at(p_uiIndex);
        return true;
    }

    void EventScript::DeleteCommand(uint32 p_uiIndex)
    {
        if (p_uiIndex >= m_pCommands.size())
            return;

        EventScriptCommandPtrList::iterator t_Itr = m_pCommands.begin();
        t_Itr += p_uiIndex;
        m_pCommands.erase(t_Itr);
    }

    void EventScript::DeleteCommand(const EventScriptCommandPtr &p_Command)
    {
        for (uint32 i = 0; i < m_pCommands.size(); ++i)
        {
            if (m_pCommands.at(i).get() == p_Command.get())
            {
                DeleteCommand(i);
                return;
            }
        }
    }

    void EventScript::DeleteCommandsOfType(ScriptCommandType p_Type)
    {
        // iterate from back
        for (uint32 i = m_pCommands.size()-1; (i+1) > 0 ; --i)
        {
            if (!m_pCommands.at(i).get())
                continue;

            if (m_pCommands.at(i).get()->GetCommandType() == p_Type)
            {
                EventScriptCommandPtrList::iterator t_Itr = m_pCommands.begin();
                t_Itr += i;
                m_pCommands.erase(t_Itr);
            }
            else if (m_pCommands.at(i).get()->GetCommandType() >= COMMAND_CONTAINER)
                ((EventScriptCommandContainer*)m_pCommands.at(i).get())->DeleteChildCommandsOfType(p_Type);
        }
    }

    bool EventScript::GetXMLData(XML_WriteData &p_Data) const
    {
        if (m_pCommands.empty())
            return false;

        p_Data.AddAttribute("Name", (LPCSTR)m_sName.c_str());

        // parse local variables
        XML_WriteData t_Variables(XML_WRITE_CHANGE);
        m_LocalVariables.GetXMLData(t_Variables);
        p_Data.AddChild("LocalVariables", t_Variables);

        // parse commands
        for (uint32 i = 0; i < m_pCommands.size() ; ++i)
        {
            XML_WriteData t_Data(XML_WRITE_APPEND);
            if (m_pCommands.at(i)->GetCommandXML(t_Data))
                p_Data.AddChild("Command", t_Data);
        }

        return true;
    }

    bool EventScript::LoadDataFromXML(const XML::XML_ReadData &p_Data)
    {
        CComVariant t_Value;
        if (p_Data.GetAttributeValue("Name", t_Value))
            m_sName = bstr_t(t_Value);

        const ReadChildList *t_pChildList = p_Data.GetChildList();
        if (!t_pChildList)
            return false;

        for (ReadChildList::const_iterator t_Itr = t_pChildList->begin(); t_Itr != t_pChildList->end(); ++t_Itr)
        {
            // get commands
            if (t_Itr->first == "Command")
            {
                if (!t_Itr->second.GetAttributeValue("Type", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) || t_Value.uintVal > EVENT_COMMAND_MAX)
                    continue;

                EventScriptCommandPtr t_pCommand;
                if (!EventScriptHolder::GetNewCommand((ScriptCommandType)t_Value.uintVal, t_pCommand))
                    continue;

                if (!t_pCommand->LoadDataFromXML(t_Itr->second))
                    continue;

                m_pCommands.push_back(t_pCommand);
            }
            // get variables
            else if (t_Itr->first == "LocalVariables")
                m_LocalVariables.LoadDataFromXML(t_Itr->second);
        }

        return true;
    }

    /*#####
    # EventScriptHolder
    #####*/
    EventScript* EventScriptHolder::InsertEventScript(int p_uiIndex)
    {
        if (p_uiIndex >= 0 && (uint32)p_uiIndex < m_Scripts.size())
        {
	        EventScriptList::iterator t_Itr = m_Scripts.begin();
	        t_Itr += p_uiIndex;
	        return &(*m_Scripts.insert(t_Itr, EventScript()));
        }
        else
        {
            m_Scripts.push_back(EventScript());
            return &m_Scripts.at(m_Scripts.size()-1);
        }
    }

    EventScript* EventScriptHolder::GetEventScript(uint32 p_uiIndex)
    {
        if (p_uiIndex >= m_Scripts.size())
            return NULL;

        return &m_Scripts.at(p_uiIndex);
    }

    int32 EventScriptHolder::GetEventScriptIndex(const EventScript *p_pEventScript)
    {
        if (!p_pEventScript)
            return -1;

        for (uint32 i = 0; i < m_Scripts.size(); ++i)
        {
            if (&m_Scripts.at(i) == p_pEventScript)
                return i;
        }

        return -1;
    }

    void EventScriptHolder::MoveEventScript(const EventScript *p_pEventScript, uint32 p_uiNewIndex)
    {
        int32 t_Index = GetEventScriptIndex(p_pEventScript);
        if (t_Index == -1)
            return;

        MoveEventScript((uint32)t_Index, p_uiNewIndex);
    }

    void EventScriptHolder::MoveEventScript(uint32 p_uiOldIndex, uint32 p_uiNewIndex)
    {
        if (p_uiOldIndex >= m_Scripts.size() || p_uiNewIndex >= m_Scripts.size())
            return;

        EventScript t_EventScript = m_Scripts.at(p_uiNewIndex);
        m_Scripts.at(p_uiNewIndex) = m_Scripts.at(p_uiOldIndex);
        m_Scripts.at(p_uiOldIndex) = t_EventScript;
    }

    void EventScriptHolder::DeleteEventScript(uint32 p_uiIndex)
    {
        if (p_uiIndex >= m_Scripts.size())
            return;

        EventScriptList::iterator t_Itr = m_Scripts.begin();
        t_Itr += p_uiIndex;
        m_Scripts.erase(t_Itr);
    }

    void EventScriptHolder::DeleteEventScript(const EventScript *p_pEventScript)
    {
        int32 t_Index = GetEventScriptIndex(p_pEventScript);
        if (t_Index == -1)
            return;

        DeleteEventScript((uint32)t_Index);
    }

    void EventScriptHolder::DeleteCommandsOfTypeInAllScripts(ScriptCommandType p_Type)
    {
        for (uint32 i = 0; i < m_Scripts.size(); ++i)
            m_Scripts.at(i).DeleteCommandsOfType(p_Type);
    }

    void EventScriptHolder::SetScriptName(const std::string &p_sScriptName, const uint32 &p_uiIndex)
    {
        if (p_uiIndex >= m_Scripts.size())
            return;

        m_Scripts.at(p_uiIndex).SetName(p_sScriptName);
    }

    bool EventScriptHolder::GetScriptName(const uint32 &p_uiIndex, std::string &p_sResult)
    {
        if (p_uiIndex >= m_Scripts.size())
            return false;

        p_sResult = m_Scripts.at(p_uiIndex).GetName();
        return true;
    }

    bool EventScriptHolder::GetXMLData(XML_WriteData &p_Data) const
    {
        bool t_bSuccess = false;
        for (uint32 i = 0; i < m_Scripts.size(); ++i)
        {
            XML_WriteData t_Data(XML_WRITE_APPEND);
            if (m_Scripts.at(i).GetXMLData(t_Data))
            {
                p_Data.AddChild("ScriptPage", t_Data);
                t_bSuccess = true;
            }
        }

        return t_bSuccess;
    }

    void EventScriptHolder::LoadDataFromXML(const XML_ReadData &p_Data)
    {
        for (ReadChildList::const_iterator t_Itr = p_Data.GetChildList()->begin(); t_Itr != p_Data.GetChildList()->end(); ++t_Itr)
        {
            if (t_Itr->first == "ScriptPage")
            {
                EventScript t_Script;
                if (t_Script.LoadDataFromXML(t_Itr->second))
                    m_Scripts.push_back(t_Script);
            }
        }
    }

    bool EventScriptHolder::GetNewCommand(ScriptCommandType p_Type, EventScriptCommandPtr &p_Result)
    {
        switch (p_Type)
        {
        case COMMAND_COMMENT: p_Result = EventScriptCommandPtr(new EventScriptComment()); return true;
        case COMMAND_CHANGE_VARIABLE: p_Result = EventScriptCommandPtr(new EventScriptChangeVariable()); return true;
        case COMMAND_CONTAINER_IF_CONDITION: p_Result = EventScriptCommandPtr(new EventScriptIfCondition()); return true;
        }

        return false;
    }
}