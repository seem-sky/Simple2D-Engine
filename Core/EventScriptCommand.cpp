#include "EventScriptCommand.h"
#include "GlobalVariables.h"

namespace EVENT_SCRIPT
{
    /*#####
    # EventScriptCommand - Superclass
    #####*/
    EventScriptCommand::EventScriptCommand()
    {
        m_CommandType = COMMAND_NONE;
    }

    EventScriptCommand::~EventScriptCommand(void)
    {
    }

    XML::XML_WriteData EventScriptCommand::GetCommandXML() const
    {
        XML::XML_WriteData t_Element;
        t_Element.AddAttribute("Type", (uint32)m_CommandType);
        return t_Element;
    }

    /*#####
    # CommandChangeVariable
    #####*/
    std::string CommandChangeVariable::GetCommandText() const
    {
        std::string t_sText = GetCommandName() + " <> ";
        std::string t_SrcVar;
        std::string t_SrcVal;
        std::string t_DestVar;
        std::string t_DestVal;
        std::string t_VarType;
        std::string t_Operator;

        // set text for dest and src var
        for (uint32 t_I = 0; t_I < 2; ++t_I)
        {
            // variable localisation
            switch (t_I ? m_DestVariable.m_VarLocalisation : m_SrcVariable.m_VarLocalisation)
            {
            case LOCALISATION_GLOBAL:
                t_I ? t_DestVar : t_SrcVar = "Global ";
                t_I ? t_DestVal : t_SrcVal = "ID " + ToString(t_I ? m_DestVariable.Type.m_GlobalVariable.m_uiVariableID :
                                                        m_SrcVariable.Type.m_GlobalVariable.m_uiVariableID) + " ";
                break;
            case LOCALISATION_VALUE:
                t_I ? t_DestVar : t_SrcVar = "Value ";
                // variable type
                switch (m_VariableType)
                {
                case VARIABLE_BOOL:
                        t_SrcVal = m_SrcVariable.Type.m_Value.m_VariableType.m_BoolValue ? "false " : "true ";
                    break;
                case VARIABLE_INT:
                    t_SrcVal = ToString(m_SrcVariable.Type.m_Value.m_VariableType.m_IntValue) + " ";
                    break;
                case VARIABLE_FLOAT:
                    t_SrcVal = ToString(m_SrcVariable.Type.m_Value.m_VariableType.m_FloatValue) + " ";
                    break;
                case VARIABLE_STRING:
                    t_SrcVal = "\"";
                    t_SrcVal += bstr_t(m_SrcVariable.Type.m_Value.m_VariableType.m_StringValue);
                    t_SrcVal += "\" ";
                    break;
                }
                break;
            }
        }

        // dest variable type
        switch (m_VariableType)
        {
        case VARIABLE_BOOL:
            t_VarType = " bool ";
            break;
        case VARIABLE_INT:
            t_VarType = " int ";
            break;
        case VARIABLE_FLOAT:
            t_VarType = " float ";
            break;
        case VARIABLE_STRING:
            t_VarType = " string ";
            break;
        }

        // operator
        switch (m_Operator)
        {
        case OPERATOR_SET:
            t_sText += "Set " + t_VarType + t_DestVar + "to " + t_SrcVar + t_SrcVal;
            break;
        case OPERATOR_ADD:
            t_sText += "Add " + t_SrcVar + t_SrcVal + "to " + t_DestVar;
            break;
        case OPERATOR_SUBTRACT:
            t_sText += "Subtract " + t_SrcVar + t_SrcVal + "from " + t_DestVar;
            break;
        case OPERATOR_MULTIPLICATE:
            t_sText += "Multiplicate " + t_DestVar + "with " + t_SrcVar + t_SrcVal;
            break;
        case OPERATOR_DIVIDE:
            t_sText += "Divide " + t_DestVar + "through " + t_SrcVar + t_SrcVal;
            break;
        case OPERATOR_MODULO:
            t_sText += "Modulo " + t_SrcVar + t_SrcVal + "from " + t_DestVar;
            break;
        case OPERATOR_TRIGGER:
            t_sText += "Trigger " + t_DestVar;
            break;
        }

        return t_sText;
    }

    XML::XML_WriteData CommandChangeVariable::GetCommandXML() const
    {
        XML::XML_WriteData t_Element = EventScriptCommand::GetCommandXML();
        t_Element.AddAttribute("Operator", (uint32)m_Operator);
        t_Element.AddAttribute("VariableType", (uint32)m_VariableType);

        for (uint32 t_I = 0; t_I < 2; ++t_I)
        {
            const ChangeConditions *t_pVar = NULL;
            if (t_I == 0)
                t_pVar = &m_DestVariable;
            else
                t_pVar = &m_SrcVariable;

            if (!t_pVar)
                continue;

            XML::XML_WriteData t_Var;
            t_Var.AddAttribute("Localisation", (uint32)t_pVar->m_VarLocalisation);
            switch (t_pVar->m_VarLocalisation)
            {
            case LOCALISATION_VALUE:
                switch (m_VariableType)
                {
                case VARIABLE_BOOL:
                    t_Var.AddAttribute("Value", t_pVar->Type.m_Value.m_VariableType.m_BoolValue);
                    break;
                case VARIABLE_INT:
                    t_Var.AddAttribute("Value", t_pVar->Type.m_Value.m_VariableType.m_IntValue);
                    break;
                case VARIABLE_FLOAT:
                    t_Var.AddAttribute("Value", t_pVar->Type.m_Value.m_VariableType.m_FloatValue);
                    break;
                case VARIABLE_STRING:
                    t_Var.AddAttribute("Value", t_pVar->Type.m_Value.m_VariableType.m_StringValue);
                    break;
                }
                break;
            case LOCALISATION_GLOBAL:
                    t_Var.AddAttribute("ID", t_pVar->Type.m_GlobalVariable.m_uiVariableID);
                break;
            }

            if (t_I == 0)
                t_Element.AddChild("DestVar", t_Var);
            else
                t_Element.AddChild("SrcVar", t_Var);
        }

        return t_Element;
    }
}