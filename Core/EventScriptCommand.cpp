#include "EventScriptCommand.h"
#include "StringAdditions.h"
#include "EventScriptHolder.h"

namespace EVENT_SCRIPT
{
    /*#####
    # EventScriptCommand - Superclass
    #####*/
    bool EventScriptCommand::GetCommandXML(XML::XML_WriteData &p_Data) const
    {
        p_Data.AddAttribute("Type", (uint32)m_CommandType);
        return true;
    }

    bool EventScriptCommand::LoadDataFromXML(const XML::XML_ReadData &p_Data)
    {
        ATL::CComVariant t_Value;
        if (!p_Data.GetAttributeValue("Type", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) || t_Value.uintVal > EVENT_COMMAND_MAX)
            return false;

        m_CommandType = (ScriptCommandType)t_Value.uintVal;
        return true;
    }

    /*#####
    # EventScriptComment
    #####*/
    bool EventScriptComment::GetCommandXML(XML::XML_WriteData &p_Data) const
    {
        if (!EventScriptCommand::GetCommandXML(p_Data))
            return false;

        p_Data.AddAttribute("Comment", (LPCSTR)m_sCommentText.c_str());
        return true;
    }

    bool EventScriptComment::LoadDataFromXML(const XML::XML_ReadData &p_Data)
    {
        if (!EventScriptCommand::LoadDataFromXML(p_Data))
            return false;

        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("Comment", t_Value))
            return false;

        m_sCommentText = bstr_t(t_Value);
        return true;
    }

    /*#####
    # EventScriptChangeVariable
    #####*/
    void EventScriptChangeVariable::SetVariableType(VariableType p_Type)
    {
        if (m_VariableType == p_Type)
            return;

        m_SrcVariable.m_VariableType = p_Type;
        m_DestVariable.m_VariableType = p_Type;
        m_VariableType = p_Type;
    }
    std::string EventScriptChangeVariable::GetCommandText() const
    {
        std::string t_sText = "<> " + GetCommandName() + ": ";
        std::string t_SrcVar;
        std::string t_SrcVal;
        std::string t_DestVar;
        std::string t_DestVal;
        std::string t_VarType;
        std::string t_Operator;

        t_SrcVar = m_SrcVariable.GetVariableSettingText();
        t_DestVar = m_DestVariable.GetVariableSettingText();

        // operator
        switch (m_Operator)
        {
        case VARIABLE_OPERATOR_SET:
            t_sText += t_DestVar+ "= " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_ADD:
            t_sText += t_DestVar + "+= " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_SUBTRACT:
            t_sText += t_DestVar + "-= " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_MULTIPLICATE:
            t_sText += t_DestVar + "x= " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_DIVIDE:
            t_sText += t_DestVar + "/= " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_MODULO:
            t_sText += t_DestVar + "%= " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_TOGGLE:
            t_sText += "Toggle " + t_DestVar;
            break;
        }

        return t_sText;
    }

    bool EventScriptChangeVariable::GetCommandXML(XML::XML_WriteData &p_Data) const
    {
        if (!EventScriptCommand::GetCommandXML(p_Data))
            return false;

        p_Data.AddAttribute("Operator", (uint32)m_Operator);
        p_Data.AddAttribute("VariableType", (uint32)m_VariableType);

        // get variables
        XML::XML_WriteData t_DestVar(XML::XML_WRITE_CHANGE);
        if (m_DestVariable.GetVariableXMLData(t_DestVar))
            p_Data.AddChild("DestVar", t_DestVar);

        XML::XML_WriteData t_SrcVar(XML::XML_WRITE_CHANGE);
        if (m_DestVariable.GetVariableXMLData(t_SrcVar))
            p_Data.AddChild("SrcVar", t_SrcVar);

        return true;
    }

    bool EventScriptChangeVariable::LoadDataFromXML(const XML::XML_ReadData &p_Data)
    {
        if (!EventScriptCommand::LoadDataFromXML(p_Data))
            return false;

        ATL::CComVariant t_Value;
        // operator
        if (!p_Data.GetAttributeValue("Operator", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) ||
            t_Value.uintVal > VARIABLE_OPERATOR_MAX)
            return false;
        m_Operator = (VariableOperator)t_Value.uintVal;

        // variable type
        if (!p_Data.GetAttributeValue("VariableType", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) ||
            t_Value.uintVal > VARIABLE_TYPE_MAX)
            return false;
        m_VariableType = (VariableType)t_Value.uintVal;

        // dest var
        const XML::XML_ReadData *t_DestData = p_Data.GetChild("DestVar");
        if (!t_DestData || !m_DestVariable.LoadDataFromXML(*t_DestData))
            return false;

        // src var
        const XML::XML_ReadData *t_SrcData = p_Data.GetChild("SrcVar");
        if (!t_SrcData || !m_SrcVariable.LoadDataFromXML(*t_SrcData))
            return false;

        return true;
    }

    /*#####
    # EventScriptCommandContainer superclass
    #####*/
    void EventScriptCommandContainer::InsertChildCommand(int p_Row, const EventScriptCommandPtr &p_pCommand)
    {
        EventScriptCommandPtrList::iterator t_Itr = m_pChildCommands.begin();
        if (p_Row > 0)
            t_Itr+p_Row;
        m_pChildCommands.insert(t_Itr, p_pCommand);
    }

    void EventScriptCommandContainer::DeleteChildCommand(const EventScriptCommandPtr &p_pCommand)
    {
        for (uint32 i = 0; i < m_pChildCommands.size(); ++i)
        {
            if (m_pChildCommands.at(i).get() == p_pCommand.get())
            {
                DeleteChildCommand(i);
                return;
            }
        }
    }

    void EventScriptCommandContainer::DeleteChildCommand(uint32 p_uiRow)
    {
        if (p_uiRow >= m_pChildCommands.size())
            return;

        EventScriptCommandPtrList::iterator t_Itr = m_pChildCommands.begin();
        t_Itr+p_uiRow;
        m_pChildCommands.erase(t_Itr);
    }

    bool EventScriptCommandContainer::GetChildCommand(uint32 p_uiIndex, EventScriptCommandPtr &p_pResult)
    {
        if (p_uiIndex >= m_pChildCommands.size())
            return false;

        p_pResult = m_pChildCommands.at(p_uiIndex);
        return true;
    }

    void EventScriptCommandContainer::DeleteChildCommandsOfType(EVENT_SCRIPT::ScriptCommandType p_Type)
    {
        // iterate from back
        for (uint32 i = m_pChildCommands.size()-1; (i+1) > 0 ; --i)
        {
            if (!m_pChildCommands.at(i).get())
                continue;

            if (m_pChildCommands.at(i).get()->GetCommandType() == p_Type)
            {
                EventScriptCommandPtrList::iterator t_Itr = m_pChildCommands.begin();
                t_Itr += i;
                m_pChildCommands.erase(t_Itr);
            }
            else if (m_pChildCommands.at(i).get()->GetCommandType() >= COMMAND_CONTAINER)
                ((EventScriptCommandContainer*)m_pChildCommands.at(i).get())->DeleteChildCommandsOfType(p_Type);
        }
    }

    void EventScriptCommandContainer::GetChildCommandXMLData(XML::XML_WriteData &p_Data) const
    {
        for (uint32 i = 0; i < m_pChildCommands.size(); ++i)
        {
            XML::XML_WriteData t_Data(XML::XML_WRITE_APPEND);
            if (m_pChildCommands.at(i)->GetCommandXML(t_Data))
                p_Data.AddChild("ChildCommand", t_Data);
        }
    }

    void EventScriptCommandContainer::LoadChildCommandsFromXML(const XML::XML_ReadData &p_Data)
    {
        const XML::ReadChildList *t_ChildList = p_Data.GetChildList();
        if (!t_ChildList)
            return;

        CComVariant t_Value;
        for (XML::ReadChildList::const_iterator t_Itr = t_ChildList->begin(); t_Itr != t_ChildList->end(); ++t_Itr)
        {
            // get commands
            if (t_Itr->first == "ChildCommand")
            {
                if (!t_Itr->second.GetAttributeValue("Type", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) ||
                    t_Value.uintVal > EVENT_COMMAND_MAX)
                    continue;

                EventScriptCommandPtr t_pCommand;
                if (!EventScriptHolder::GetNewCommand((ScriptCommandType)t_Value.uintVal, t_pCommand))
                    continue;

                if (!t_pCommand->LoadDataFromXML(t_Itr->second))
                    continue;

                m_pChildCommands.push_back(t_pCommand);
            }
        }
    }

    /*#####
    # EventScriptIfCondition
    #####*/
    EventScriptIfCondition::EventScriptIfCondition() : EventScriptCommandContainer()
    {
        m_CommandType = COMMAND_CONTAINER_IF_CONDITION;
    }

    std::string EventScriptIfCondition::GetCommandText() const
    {
        std::string t_sText = "<> " + GetCommandName() + " ";
        std::string t_sConditionText;
        if (m_ConditionHolder.GetConditionText(t_sConditionText))
            t_sText.append(t_sConditionText);

        return t_sText;
    }

    bool EventScriptIfCondition::GetCommandXML(XML::XML_WriteData &p_Data) const
    {
        if (!EventScriptCommand::GetCommandXML(p_Data))
            return false;

        p_Data.AddAttribute("Comparison", (uint32)m_ConditionHolder.GetComparison());
        for (std::size_t i = 0; i < m_ConditionHolder.GetConditionCount(); ++i)
        {
            XML::XML_WriteData t_Data(XML::XML_WRITE_APPEND);
            if (m_ConditionHolder[i]->GetXMLData(t_Data))
                p_Data.AddChild("Condition", t_Data);
        }
        GetChildCommandXMLData(p_Data);
        return true;
    }

    bool EventScriptIfCondition::LoadDataFromXML(const XML::XML_ReadData &p_Data)
    {
        if (!EventScriptCommand::LoadDataFromXML(p_Data))
            return false;

        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("Comparison", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) || t_Value.uintVal > ConditionHolder::COMPARISON_MAX)
            return false;
        m_ConditionHolder.SetComparison((ConditionHolder::Comparison)t_Value.uintVal);

        const XML::ReadChildList *t_pChildList = p_Data.GetChildList();
        if (!t_pChildList)
            return false;

        for (XML::ReadChildList::const_iterator t_Itr = t_pChildList->begin(); t_Itr != t_pChildList->end(); ++t_Itr)
        {
            if (t_Itr->first == "Condition")
            {
                ConditionSettingsPtr t_Condition(new ConditionSettings());
                if (t_Condition->LoadDataFromXML(t_Itr->second))
                    m_ConditionHolder.AddCondition(t_Condition);
            }
        }

        LoadChildCommandsFromXML(p_Data);
        return true;
    }
}