#include "CommandWindow.h"
#include "EventEditorWidget.h"
#include "moc_CommandWindow.h"

using namespace EVENT_SCRIPT;

CommandWindow::CommandWindow(QWidget *p_pParent): QDialog(p_pParent), Ui_CommandDialog()
{
    setupUi(this);

    connect(m_pButtonChangeVariable, SIGNAL(clicked()), this, SLOT(ChangeVariableClicked()));
    connect(m_pButtonComment, SIGNAL(clicked()), this, SLOT(CommentClicked()));
    connect(m_pButtonIfCondition, SIGNAL(clicked()), this, SLOT(IfConditionClicked()));
}

bool CommandWindow::ExecCorrectCommandWindow(EventScriptCommand *p_pCommand, ScriptPage *p_pParent)
{
    if (!p_pCommand || !p_pParent)
        return false;

    EventScriptCommandDialog *t_pDialog = NULL;
    switch (p_pCommand->GetCommandType())
    {
    case COMMAND_COMMENT:
        t_pDialog = new EventScriptCommentDialog((EventScriptComment*)p_pCommand, p_pParent);
    	break;

    case COMMAND_CHANGE_VARIABLE:
        t_pDialog = new EventScriptChangeVariableDialog((EventScriptChangeVariable*)p_pCommand, p_pParent);
        break;

    case COMMAND_CONTAINER_IF_CONDITION:
        t_pDialog = new EventScriptIfConditionDialog((EventScriptIfCondition*)p_pCommand, p_pParent);
        break;
    }

    if (t_pDialog)
    {
        t_pDialog->LoadValuesFromEventScriptCommand();
        if (t_pDialog->exec() == QDialog::Accepted)
            return true;
    }

    return false;
}

void CommandWindow::ChangeVariableClicked()
{
    if (!parent())
        return;

    accept();
    EventScriptChangeVariable t_Command;
    EventScriptChangeVariableDialog t_Dialog(&t_Command, (QWidget*)parent());
    if (t_Dialog.exec() == QDialog::Accepted)
    {
        if (ScriptPage *t_Page = (ScriptPage*)parent())
            t_Page->AddNewCommandLine(new EventScriptChangeVariable(t_Command), t_Page->indexOfTopLevelItem(t_Page->currentItem()));
    }
}

void CommandWindow::CommentClicked()
{
    if (!parent())
        return;

    accept();
    EventScriptComment t_Command;
    EventScriptCommentDialog t_Dialog(&t_Command, (QWidget*)parent());
    if (t_Dialog.exec() == QDialog::Accepted)
    {
        if (ScriptPage *t_Page = (ScriptPage*)parent())
            t_Page->AddNewCommandLine(new EventScriptComment(t_Command), t_Page->indexOfTopLevelItem(t_Page->currentItem()));
    }
}

void CommandWindow::IfConditionClicked()
{
    if (!parent())
        return;

    accept();
    EventScriptIfCondition t_Command;
    EventScriptIfConditionDialog t_Dialog(&t_Command, (QWidget*)parent());
    if (t_Dialog.exec() == QDialog::Accepted)
    {
        if (ScriptPage *t_Page = (ScriptPage*)parent())
            t_Page->AddNewCommandLine(new EventScriptIfCondition(t_Command), t_Page->indexOfTopLevelItem(t_Page->currentItem()));
    }
}