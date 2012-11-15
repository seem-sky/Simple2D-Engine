#include "EventScriptCommand.h"

namespace EVENT_SCRIPT
{
    /*#####
    # VariableSettings
    #####*/
    std::string VariableSettings::GetVariableTypeText() const
    {
        switch(m_VariableType)
        {
        case VARIABLE_BOOL:
            return "Bool";
        case VARIABLE_INT:
            return "Integer";
        case VARIABLE_FLOAT:
            return "Float";
        case VARIABLE_STRING:
            return "String";
        }

        return "";
    }

    std::string VariableSettings::GetVariableSettingText() const
    {
        // variable localisation
        switch (m_VarLocalisation)
        {
        case LOCALISATION_LOCAL:
            return "Local " + GetVariableTypeText() + " ID " + ToString(m_Type.m_LocalVariable.m_uiVariableID) + " ";
        case LOCALISATION_GLOBAL:
            return "Global " + GetVariableTypeText() + " ID " + ToString(m_Type.m_GlobalVariable.m_uiVariableID) + " ";
        case LOCALISATION_VALUE:
            {
                std::string t_sText = "Value " + GetVariableTypeText() + " ";
                // variable type
                switch (m_VariableType)
                {
                case VARIABLE_BOOL:
                    t_sText += m_Type.m_Value.m_VariableType.m_BoolValue ? "true " : "false ";
                    break;
                case VARIABLE_INT:
                    t_sText += ToString(m_Type.m_Value.m_VariableType.m_IntValue) + " ";
                    break;
                case VARIABLE_FLOAT:
                    t_sText += ToString(m_Type.m_Value.m_VariableType.m_FloatValue) + " ";
                    break;
                case VARIABLE_STRING:
                    t_sText += "\"";
                    t_sText += bstr_t(m_Type.m_Value.m_VariableType.m_StringValue);
                    t_sText += "\" ";
                    break;
                }
                return t_sText;
            }
        }

        return "";
    }

    /*#####
    # EventScriptCommand - Superclass
    #####*/
    XML::XML_WriteData EventScriptCommand::GetCommandXML() const
    {
        XML::XML_WriteData t_Element(XML::XML_WRITE_APPEND);
        t_Element.AddAttribute("Type", (uint32)m_CommandType);
        return t_Element;
    }

    /*#####
    # EventScriptComment
    #####*/
    XML::XML_WriteData EventScriptComment::GetCommandXML() const
    {
        XML::XML_WriteData t_Element;
        t_Element.AddAttribute("Type", (uint32)m_CommandType);
        t_Element.AddAttribute("Comment", ((LPCOLESTR)_bstr_t(m_sCommentText.c_str())));
        return t_Element;
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
            t_sText += t_DestVar + "+ " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_SUBTRACT:
            t_sText += t_DestVar + "- " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_MULTIPLICATE:
            t_sText += t_DestVar + "x " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_DIVIDE:
            t_sText += t_DestVar + "/ " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_MODULO:
            t_sText += t_DestVar + "% " + t_SrcVar;
            break;
        case VARIABLE_OPERATOR_TRIGGER:
            t_sText += "Trigger " + t_DestVar;
            break;
        }

        return t_sText;
    }

    XML::XML_WriteData EventScriptChangeVariable::GetCommandXML() const
    {
        XML::XML_WriteData t_Element = EventScriptCommand::GetCommandXML();
        t_Element.AddAttribute("Operator", (uint32)m_Operator);
        t_Element.AddAttribute("VariableType", (uint32)m_VariableType);

        for (uint32 t_I = 0; t_I < 2; ++t_I)
        {
            const VariableSettings *t_pVar = NULL;
            t_pVar = t_I == 0 ? &m_DestVariable : &m_SrcVariable;
            if (!t_pVar)
                continue;

            XML::XML_WriteData t_Var(XML::XML_WRITE_CHANGE);
            t_Var.AddAttribute("Localisation", (uint32)t_pVar->m_VarLocalisation);
            switch (t_pVar->m_VarLocalisation)
            {
            case LOCALISATION_VALUE:
                switch (m_VariableType)
                {
                case VARIABLE_BOOL:
                    t_Var.AddAttribute("Value", t_pVar->m_Type.m_Value.m_VariableType.m_BoolValue);
                    break;
                case VARIABLE_INT:
                    t_Var.AddAttribute("Value", t_pVar->m_Type.m_Value.m_VariableType.m_IntValue);
                    break;
                case VARIABLE_FLOAT:
                    t_Var.AddAttribute("Value", t_pVar->m_Type.m_Value.m_VariableType.m_FloatValue);
                    break;
                case VARIABLE_STRING:
                    t_Var.AddAttribute("Value", t_pVar->m_Type.m_Value.m_VariableType.m_StringValue);
                    break;
                }
                break;
            case LOCALISATION_GLOBAL:
                t_Var.AddAttribute("ID", t_pVar->m_Type.m_GlobalVariable.m_uiVariableID);
                break;
            case LOCALISATION_LOCAL:
                t_Var.AddAttribute("ID", t_pVar->m_Type.m_LocalVariable.m_uiVariableID);
                break;
            }

            if (t_I == 0)
                t_Element.AddChild("DestVar", t_Var);
            else
                t_Element.AddChild("SrcVar", t_Var);
        }

        return t_Element;
    }

    /*#####
    # EventScriptCommandContainer superclass
    #####*/
    EventScriptCommandContainer::~EventScriptCommandContainer()
    {
        for (uint32 i = 0; i < m_pChildCommands.size(); ++i)
        {
            if (m_pChildCommands.at(i))
            {
                delete m_pChildCommands.at(i);
                m_pChildCommands.at(i) = NULL;
            }
        }
    }

    void EventScriptCommandContainer::InsertChildCommand(int p_uiRow, EventScriptCommand *p_pCommand)
    {
        if (!p_pCommand)
            return;

        CommandList::iterator t_Itr = m_pChildCommands.begin();
        if (p_uiRow > 0)
            t_Itr+p_uiRow;
        m_pChildCommands.insert(t_Itr, p_pCommand);
    }

    /*#####
    # EventScriptIfCondition
    #####*/
    EventScriptIfCondition::EventScriptIfCondition() : m_ConditionType(CONDITION_TYPE_NONE), m_Operator(CONDITION_OPERATOR_EQUAL),
        EventScriptCommandContainer()
    {
        m_CommandType = COMMAND_CONTAINER_IF_CONDITION;
        memset(&m_Type, NULL, sizeof(m_Type));
    }

    EventScriptIfCondition::~EventScriptIfCondition()
    {
        SetConditionType(CONDITION_TYPE_NONE);
        EventScriptCommandContainer::~EventScriptCommandContainer();
    }

    std::string EventScriptIfCondition::GetCommandText() const
    {
        std::string t_sText = "<> " + GetCommandName() + "If ";
        std::string t_sType;
        std::string t_sOperator;
        switch(m_Operator)
        {
        case CONDITION_OPERATOR_EQUAL: t_sOperator = "== "; break;
        case CONDITION_OPERATOR_NOT_EQUAL: t_sOperator = "!= "; break;
        case CONDITION_OPERATOR_GREATER_EQUAL: t_sOperator = ">= "; break;
        case CONDITION_OPERATOR_LESS_EQUAL: t_sOperator = "<= "; break;
        case CONDITION_OPERATOR_GREATER: t_sOperator = "> "; break;
        case CONDITION_OPERATOR_LESS: t_sOperator = "< "; break;
        }

        switch (m_ConditionType)
        {
        case CONDITION_TYPE_VARIABLE:
            t_sType = "Variable ";
            std::string t_sDestText;
            std::string t_sSrcText;
            if (m_Type.m_ConditionVariable.m_pDestVar)
                t_sDestText = m_Type.m_ConditionVariable.m_pDestVar->GetVariableSettingText();
            if (m_Type.m_ConditionVariable.m_pSrcVar)
                t_sSrcText = m_Type.m_ConditionVariable.m_pSrcVar->GetVariableSettingText();
            break;
        }

        return t_sText;
    }

    void EventScriptIfCondition::SetConditionType(ConditionType p_Type)
    {
        if (m_ConditionType != p_Type)
        {
            m_ConditionType = p_Type;
            switch(m_ConditionType)
            {
            case CONDITION_TYPE_VARIABLE:
                if (m_Type.m_ConditionVariable.m_pDestVar)
                    delete m_Type.m_ConditionVariable.m_pDestVar;
                if (m_Type.m_ConditionVariable.m_pSrcVar)
                    delete m_Type.m_ConditionVariable.m_pSrcVar;
                break;
            }
            memset(&m_Type, NULL, sizeof(m_Type));
        }
    }

    const VariableSettings* EventScriptIfCondition::GetDestinationVariable() const
    {
        if (m_ConditionType != CONDITION_TYPE_VARIABLE)
            return NULL;

        return m_Type.m_ConditionVariable.m_pDestVar;
    }

    const VariableSettings* EventScriptIfCondition::GetSourceVariable() const
    {
        if (m_ConditionType != CONDITION_TYPE_VARIABLE)
            return NULL;

        return m_Type.m_ConditionVariable.m_pSrcVar;
    }

    void EventScriptIfCondition::SetDestinationVariable(VariableSettings &p_Variable)
    {
        if (m_ConditionType != CONDITION_TYPE_VARIABLE)
            return;

        if (m_Type.m_ConditionVariable.m_pDestVar)
            *m_Type.m_ConditionVariable.m_pDestVar = p_Variable;
        else
            m_Type.m_ConditionVariable.m_pDestVar = new VariableSettings(p_Variable);
    }

    void EventScriptIfCondition::SetSourceVariable(VariableSettings &p_Variable)
    {
        if (m_ConditionType != CONDITION_TYPE_VARIABLE)
            return;

        if (m_Type.m_ConditionVariable.m_pSrcVar)
            *m_Type.m_ConditionVariable.m_pSrcVar = p_Variable;
        else
            m_Type.m_ConditionVariable.m_pSrcVar = new VariableSettings(p_Variable);
    }
}