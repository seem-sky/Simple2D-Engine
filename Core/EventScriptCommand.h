#ifndef EVENT_SCRIPT_COMMAND_H
#define EVENT_SCRIPT_COMMAND_H

#include <map>
#include "Global.h"
#include "Database.h"
#include <atlcomcli.h>
#include "XML_Writer.h"

namespace EVENT_SCRIPT
{
    /*#####
    # EventCommand superclass
    #####*/
    enum ScriptCommandType
    {
        COMMAND_NONE,
        COMMAND_CHANGE_VARIABLE,
        COMMAND_IF_CONDITION,
        COMMAND_LOOP,
    };

    class EventScriptCommand
    {
    public:
        EventScriptCommand();
        ~EventScriptCommand(void);

        inline ScriptCommandType GetCommandType() const { return m_CommandType; }
        virtual std::string GetCommandName() const = NULL;
        virtual std::string GetCommandText() const = NULL;
        virtual XML::XML_WriteData GetCommandXML() const;

    protected:
        ScriptCommandType m_CommandType;
    };

    typedef std::map<uint32, EventScriptCommand> CommandList;

    /*#####
    # Change Variable
    #####*/
    enum VariableOperator
    {
        OPERATOR_SET,
        OPERATOR_ADD,
        OPERATOR_SUBTRACT,
        OPERATOR_MULTIPLICATE,
        OPERATOR_DIVIDE,
        OPERATOR_MODULO,
        OPERATOR_TRIGGER,
    };

    enum VariableLocalisation
    {
        LOCALISATION_NONE,
        LOCALISATION_VALUE,
        LOCALISATION_GLOBAL,
        LOCALISATION_LOCAL,
        LOCALISATION_OBJECT,
    };

    struct ChangeConditions
    {
        ChangeConditions() : m_VarLocalisation(LOCALISATION_NONE)
        {
            memset(&Type, 0, sizeof(Type));
        }

        VariableLocalisation m_VarLocalisation;

        union VariableChangeType
        {
            struct Value                    // type 0
            {
                union VariableType
                {
                    bool m_BoolValue;
                    int m_IntValue;
                    float m_FloatValue;
                    BSTR m_StringValue;
                } m_VariableType;
            } m_Value;

            struct GlobalVariable           // type 1
            {
                uint32 m_uiVariableID;
            } m_GlobalVariable;
        } Type;
    };

    class CommandChangeVariable : public EventScriptCommand
    {
    public:
        CommandChangeVariable() : m_Operator(OPERATOR_SET), EventScriptCommand()
        {
            m_CommandType = COMMAND_CHANGE_VARIABLE;
        }

        virtual std::string GetCommandText() const;
        virtual std::string GetCommandName() const { return "Change Variable"; }
        virtual XML::XML_WriteData GetCommandXML() const;

        VariableOperator GetOperator() const { return m_Operator; }
        void SetOperator(VariableOperator p_Operator) { m_Operator = p_Operator; }
        void SetVariableType(VariableType p_Type) { m_VariableType = p_Type; }
        VariableType GetVariableType() { return m_VariableType; }

        inline ChangeConditions GetDestVariable() const { return m_DestVariable; }
        inline void SetDestVariable(ChangeConditions m_Var) { m_DestVariable = m_Var; }
        inline ChangeConditions GetSrcVariable() const { return m_SrcVariable; }
        void SetSrcVariable(ChangeConditions m_Var) { m_SrcVariable = m_Var; }

    private:
        VariableOperator m_Operator;
        VariableType m_VariableType;

        ChangeConditions m_SrcVariable;
        ChangeConditions m_DestVariable;
    };
}
#endif