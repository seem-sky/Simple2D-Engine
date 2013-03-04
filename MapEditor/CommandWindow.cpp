#include "CommandWindow.h"
#include "EventEditorWidget.h"
#include "moc_CommandWindow.h"
#include "IndexDialog.h"

using namespace EVENT_SCRIPT;

CommandWindow::CommandWindow(ScriptContainer *p_pParent, ScriptPage *p_pWidgetParent): m_pParent(p_pParent), QDialog(p_pWidgetParent), Ui_CommandDialog()
{
    setupUi(this);

    connect(m_pButtonChangeVariable, SIGNAL(clicked()), this, SLOT(ChangeVariableClicked()));
    connect(m_pButtonComment, SIGNAL(clicked()), this, SLOT(CommentClicked()));
    connect(m_pButtonIfCondition, SIGNAL(clicked()), this, SLOT(IfConditionClicked()));
}

bool CommandWindow::ExecCorrectCommandWindow(const EventScriptCommandPtr &p_pCommand, ScriptPage *p_pParent)
{
    if (!p_pParent || !p_pCommand.get())
        return false;

    EventScriptCommandDialog *t_pDialog = NULL;
    switch (p_pCommand.get()->GetCommandType())
    {
    case COMMAND_COMMENT:
        t_pDialog = new EventScriptCommentDialog((EventScriptComment*)p_pCommand.get(), p_pParent);
    	break;

    case COMMAND_CHANGE_VARIABLE:
        t_pDialog = new EventScriptChangeVariableDialog((EventScriptChangeVariable*)p_pCommand.get(), p_pParent);
        break;

    case COMMAND_CONTAINER_IF_CONDITION:
        t_pDialog = new EventScriptIfConditionDialog((EventScriptIfCondition*)p_pCommand.get(), p_pParent);
        break;
    }

    if (t_pDialog)
    {
        t_pDialog->LoadValuesFromEventScriptCommand();
        if (t_pDialog->exec() == QDialog::Accepted)
            return true;

        delete t_pDialog;
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
    if (t_Dialog.exec() == QDialog::Accepted && m_pParent)
            m_pParent->AddNewCommandLine(EventScriptCommandPtr(new EventScriptChangeVariable(t_Command)), m_pParent->GetCurrentIndex());
}

void CommandWindow::CommentClicked()
{
    if (!parent())
        return;

    accept();
    EventScriptComment t_Command;
    EventScriptCommentDialog t_Dialog(&t_Command, (QWidget*)parent());
    if (t_Dialog.exec() == QDialog::Accepted && m_pParent)
        m_pParent->AddNewCommandLine(EventScriptCommandPtr(new EventScriptComment(t_Command)), m_pParent->GetCurrentIndex());
}

void CommandWindow::IfConditionClicked()
{
    if (!parent())
        return;

    accept();
    EventScriptIfCondition t_Command;
    EventScriptIfConditionDialog t_Dialog(&t_Command, (QWidget*)parent());
    if (t_Dialog.exec() == QDialog::Accepted && m_pParent)
        m_pParent->AddNewCommandLine(EventScriptCommandPtr(new EventScriptIfCondition(t_Command)), m_pParent->GetCurrentIndex());
}