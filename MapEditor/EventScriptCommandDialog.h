#ifndef EVENT_SCRIPT_COMMAND_DIALOG_H
#define EVENT_SCRIPT_COMMAND_DIALOG_H

#include <EventScriptCommand.h>
#include "ModifyObject.h"
#include <QtGui/QDialog>

/*#####
# EventScriptCommandDialog -> superclass
#####*/
class EventScriptCommandDialog : public QDialog
{
    Q_OBJECT
public:
    EventScriptCommandDialog(EVENT_SCRIPT::EventScriptCommand *p_pCommand, QWidget *p_pParent = NULL);

    virtual void LoadValuesFromEventScriptCommand() = NULL;

private slots:
    virtual void Accept() = NULL;

protected:
    EVENT_SCRIPT::EventScriptCommand *m_pCommand;
};

/*#####
# EventScriptCommentDialog
#####*/
#include "ui/UI_CommandComment.h"

class EventScriptCommentDialog : public EventScriptCommandDialog, Ui_CommandComment
{
    Q_OBJECT
public:
    EventScriptCommentDialog(EVENT_SCRIPT::EventScriptComment *p_pCommand, QWidget *p_pParent = NULL);
    void LoadValuesFromEventScriptCommand();

protected:
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.resizeEvent(this); }

private slots:
    void Accept();

private:
    ModifyObject m_ResizeObj;
};

/*#####
# EventScriptChangeVariableDialog
#####*/
#include "ui/UI_CommandChangeVariable.h"

class EventScriptChangeVariableDialog : public EventScriptCommandDialog, public Ui_CommandChangeVariable
{
    Q_OBJECT
public:
    EventScriptChangeVariableDialog(EVENT_SCRIPT::EventScriptChangeVariable *p_pCommand, QWidget *p_pParent = NULL);
    void LoadValuesFromEventScriptCommand();

private slots:
    void Accept();
    void ChangeVariableType(bool p_bChecked);
    void ChangeOperator(bool p_bChecked);
};

/*#####
# EventScriptIfConditionDialog
#####*/
#include "ui/UI_CommandIfCondition.h"

class EventScriptIfConditionDialog : public EventScriptCommandDialog, public Ui_CommandIfCondition
{
    Q_OBJECT
public:
    EventScriptIfConditionDialog(EVENT_SCRIPT::EventScriptIfCondition *p_pCommand, QWidget *p_pParent = NULL);
    void LoadValuesFromEventScriptCommand();

private:
    void RemoveCondition(int32 p_Index);
    void AddCondition();

private slots:
    void Accept();
    void ConditionAdded();
    void ConditionDeleted();
};
#endif