#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include "UI/UI_CommandDialog.h"
#include "EventScriptCommandDialog.h"
#include "EventEditorWidget.h"

class CommandWindow : public QDialog, public Ui_CommandDialog
{
    Q_OBJECT
public:
    CommandWindow(QWidget *p_pParent = NULL);

    static bool CommandWindow::ExecCorrectCommandWindow(EVENT_SCRIPT::EventScriptCommand *p_pCommand, ScriptPage *p_pParent);

private slots:
    void ChangeVariableClicked();
    void CommentClicked();
    void IfConditionClicked();
};
#endif