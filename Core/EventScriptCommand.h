#ifndef EVENT_SCRIPT_COMMAND_H
#define EVENT_SCRIPT_COMMAND_H

#include <vector>
#include "Global.h"
#include <atlcomcli.h>
#include "XML_Writer.h"
#include "ConditionHolder.h"

namespace EVENT_SCRIPT
{
    enum ScriptCommandType
    {
        COMMAND_NONE,
        COMMAND_COMMENT,
        COMMAND_CHANGE_VARIABLE,

        // add container commands below COMMAND_CONTAINER
        COMMAND_CONTAINER,
        COMMAND_CONTAINER_IF_CONDITION,
        COMMAND_CONTAINER_LOOP,
    };

    const uint32 EVENT_COMMAND_MAX = 5;

    /*#####
    # EventCommand superclass
    #####*/
    class EventScriptCommand
    {
    public:
        EventScriptCommand() { m_CommandType = COMMAND_NONE; }

        inline ScriptCommandType GetCommandType() const { return m_CommandType; }
        virtual std::string GetCommandName() const { return ""; }
        virtual std::string GetCommandText() const { return "<>"; }
        virtual bool GetCommandXML(XML::XML_WriteData &p_Data) const;
        virtual bool LoadDataFromXML(const XML::XML_ReadData &p_Data);

    protected:
        ScriptCommandType m_CommandType;
    };

    typedef boost::shared_ptr<EventScriptCommand> EventScriptCommandPtr;
    typedef std::vector<EventScriptCommandPtr> EventScriptCommandPtrList;

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
        bool GetCommandXML(XML::XML_WriteData &p_Data) const;
        bool LoadDataFromXML(const XML::XML_ReadData &p_Data);

    private:
        std::string m_sCommentText;
    };

    /*#####
    # Change Variable
    #####*/
    class EventScriptChangeVariable : public EventScriptCommand
    {
    public:
        EventScriptChangeVariable() : m_Operator(VARIABLE_OPERATOR_SET), EventScriptCommand()
        {
            m_CommandType = COMMAND_CHANGE_VARIABLE;
        }

        std::string GetCommandText() const;
        std::string GetCommandName() const { return "Change Variable"; }
        bool GetCommandXML(XML::XML_WriteData &p_Data) const;
        bool LoadDataFromXML(const XML::XML_ReadData &p_Data);

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

        void InsertChildCommand(int p_Row, const EventScriptCommandPtr &p_pCommand);
        void DeleteChildCommand(const EventScriptCommandPtr &p_pCommand);
        void DeleteChildCommand(uint32 p_uiRow);
        void DeleteChildCommandsOfType(ScriptCommandType p_Type);
        uint32 GetChildCommandCount() const { return m_pChildCommands.size(); }
        bool GetChildCommand(uint32 p_uiIndex, EventScriptCommandPtr &p_pResult);

    protected:
        void GetChildCommandXMLData(XML::XML_WriteData &p_Data) const;
        void LoadChildCommandsFromXML(const XML::XML_ReadData &p_Data);

    private:
        EventScriptCommandPtrList m_pChildCommands;
    };

    /*#####
    # EventScriptIfCondition
    #####*/
    class EventScriptIfCondition : public EventScriptCommandContainer
    {
    public:
        EventScriptIfCondition();

        std::string GetCommandName() const { return "If Condition: "; }

        inline const ConditionHolder* GetConditionHolder() const { return &m_ConditionHolder; }
        inline ConditionHolder* GetConditionHolder() { return &m_ConditionHolder; }
        std::string GetCommandText() const;
        bool GetCommandXML(XML::XML_WriteData &p_Data) const;
        bool LoadDataFromXML(const XML::XML_ReadData &p_Data);
    private:
        ConditionHolder m_ConditionHolder;
    };
}
#endif