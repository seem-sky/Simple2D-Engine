#include "CommandWindow.h"
#include "EventEditorWidget.h"
#include "moc_CommandWindow.h"
#include "ChangeVariableWindow.h"

using namespace EVENT_SCRIPT;

CommandWindow::CommandWindow(QWidget *p_pParent): QDialog(p_pParent), Ui_CommandDialog()
{
    setupUi(this);

    connect(m_pButtonChangeVariable, SIGNAL(clicked()), this, SLOT(ChangeVariableClicked()));
}

CommandWindow::~CommandWindow(void)
{
}

void CommandWindow::ChangeVariableClicked()
{
    if (!parent())
        return;

    accept();
    CommandChangeVariable t_Command;
    ChangeVariableWindow t_Dialog(&t_Command, (QWidget*)parent());
    if (t_Dialog.exec() == QDialog::Accepted)
    {
        if (ScriptPage *t_Page = (ScriptPage*)parent())
            t_Page->AddNewCommandLine(new CommandChangeVariable(t_Command));
    }
}