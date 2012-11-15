#ifndef EVENT_SCRIPT_COMMAND_H
#define EVENT_SCRIPT_COMMAND_H

#include <vector>
#include "Global.h"
#include "Database.h"
#include <atlcomcli.h>
#include "XML_Writer.h"

namespace EVENT_SCRIPT
{
    enum ScriptCommandType
    {
        COMMAND_NONE,
        COMMAND_COMMENT,
        COMMAND_CHANGE_VARIABLE,

        // add cotainer below COMMAND_CONTAINER
        COMMAND_CONTAINER,
        COMMAND_CONTAINER_IF_CONDITION,
        COMMAND_CONTAINER_LOOP,

        COMMAND_MAX,
    };

    /*#####
    # EventCommand superclass
    #####*/
    class EventScriptCommand
    {
    public:
        EventScriptCommand() { m_CommandType = COMMAND_NONE; }

        inline ScriptCommandType GetCommandType() const { return m_CommandType; }
        virtual std::string GetCommandName() const { return ""; }
        virtual std::string GetCommandText() const { return ""; }
        virtual XML::XML_WriteData GetCommandXML() const;

    protected:
        ScriptCommandType m_CommandType;
    };

    typedef std::vector<EventScriptCommand*> CommandList;

    /*#####
    # Comment
    #####*/
    class EventScriptComment : public EventScriptCommand
    {
    public:
        EventScriptComment() : EventScriptCommand()
        {
            m_CommandType = COMMAND_COMMENT;
        }

        void SetCommentText(std::string p_sText) { m_sCommentText = p_sText; }
        inline std::string GetCommentText() const { return m_sCommentText; }

        std::string GetCommandName() const { return "Comment"; }
        std::string GetCommandText() const { return "<> " + GetCommandName() + ": " + GetCommentText(); }
        XML::XML_WriteData GetCommandXML() const;

    private:
        std::string m_sCommentText;
    };

    /*#####
    # Change Variable
    #####*/
    enum VariableOperator
    {
        VARIABLE_OPERATOR_SET,
        VARIABLE_OPERATOR_ADD,
        VARIABLE_OPERATOR_SUBTRACT,
        VARIABLE_OPERATOR_MULTIPLICATE,
        VARIABLE_OPERATOR_DIVIDE,
        VARIABLE_OPERATOR_MODULO,
        VARIABLE_OPERATOR_TRIGGER,
    };

    enum VariableLocalisation
    {
        LOCALISATION_NONE,
        LOCALISATION_VALUE,
        LOCALISATION_GLOBAL,
        LOCALISATION_LOCAL,
        LOCALISATION_OBJECT,
    };

    struct VariableSettings
    {
        VariableSettings() : m_VarLocalisation(LOCALISATION_NONE)
        {
            memset(&m_Type, 0, sizeof(m_Type));
        }

        std::string GetVariableSettingText() const;
        std::string GetVariableTypeText() const;

        VariableLocalisation m_VarLocalisation;
        VariableType m_VariableType;

        union VariableSettingType
        {
            struct Value                    // type 1
            {
                union VariableType
                {
                    bool m_BoolValue;
                    int m_IntValue;
                    float m_FloatValue;
                    BSTR m_StringValue;
                } m_VariableType;
            } m_Value;

            struct GlobalVariable           // type 2
            {
                uint32 m_uiVariableID;
            } m_GlobalVariable;

            struct LocalVariable           // type 3
            {
                uint32 m_uiVariableID;
            } m_LocalVariable;
        } m_Type;
    };

    class EventScriptChangeVariable : public EventScriptCommand
    {
    public:
        EventScriptChangeVariable() : m_Operator(VARIABLE_OPERATOR_SET), EventScriptCommand()
        {
            m_CommandType = COMMAND_CHANGE_VARIABLE;
        }

        virtual std::string GetCommandText() const;
        virtual std::string GetCommandName() const { return "Change Variable"; }
        virtual XML::XML_WriteData GetCommandXML() const;

        VariableOperator GetOperator() const { return m_Operator; }
        void SetOperator(VariableOperator p_Operator) { m_Operator = p_Operator; }

        void SetVariableType(VariableType p_Type);
        inline VariableType GetVariableType() { return m_VariableType; }

        inline VariableSettings GetDestVariable() const { return m_DestVariable; }
        inline void SetDestVariable(VariableSettings m_Var) { m_DestVariable = m_Var; }
        inline VariableSettings GetSrcVariable() const { return m_SrcVariable; }
        void SetSrcVariable(VariableSettings m_Var) { m_SrcVariable = m_Var; }

    private:
        VariableOperator m_Operator;
        VariableType m_VariableType;

        VariableSettings m_SrcVariable;
        VariableSettings m_DestVariable;
    };

    /*#####
    # EventScriptCommandContainer superclass
    #####*/
    class EventScriptCommandContainer : public EventScriptCommand
    {
    public:
        EventScriptCommandContainer() : EventScriptCommand()
        {
            m_CommandType = COMMAND_CONTAINER;
        }

        virtual ~EventScriptCommandContainer();
        void InsertChildCommand(int p_uiRow, EventScriptCommand *p_pCommand);

    private:
        CommandList m_pChildCommands;
    };

    /*#####
    # EventScriptIfCondition
    #####*/
    enum ConditionType
    {
        CONDITION_TYPE_NONE,
        CONDITION_TYPE_VARIABLE,
    };

    enum ConditionOperator
    {
        CONDITION_OPERATOR_EQUAL,
        CONDITION_OPERATOR_NOT_EQUAL,
        CONDITION_OPERATOR_GREATER_EQUAL,
        CONDITION_OPERATOR_LESS_EQUAL,
        CONDITION_OPERATOR_GREATER,
        CONDITION_OPERATOR_LESS,
    };

    class EventScriptIfCondition : public EventScriptCommandContainer
    {
    public:
        EventScriptIfCondition();
        virtual ~EventScriptIfCondition();

        std::string GetCommandName() const { return "If Condition: "; }
        std::string GetCommandText() const;

        void SetConditionType(ConditionType p_Type);
        inline ConditionType GetConditionType() const { return m_ConditionType; }

        void SetOperator(ConditionOperator p_Operator) { m_Operator = p_Operator; }
        ConditionOperator GetOperator() const { return m_Operator; }

        const VariableSettings* GetDestinationVariable() const;
        void SetDestinationVariable(VariableSettings &p_Variable);
        const VariableSettings* GetSourceVariable() const;
        void SetSourceVariable(VariableSettings &p_Variable);

    private:
        union Type
        {
            struct ConditionVariable
            {
                VariableSettings *m_pDestVar;
                VariableSettings *m_pSrcVar;
            } m_ConditionVariable;
        } m_Type;

        ConditionType m_ConditionType;
        ConditionOperator m_Operator;
    };
}
#endif