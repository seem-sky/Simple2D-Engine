#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include "UI/UI_CommandDialog.h"
#include "EventScriptCommandDialog.h"
#include "EventEditorWidget.h"

class CommandWindow : public QDialog, public Ui_CommandDialog
{
    Q_OBJECT
public:
    CommandWindow(ScriptContainer *p_pParent, ScriptPage *p_pWidgetParent);

    static bool CommandWindow::ExecCorrectCommandWindow(const EVENT_SCRIPT::EventScriptCommandPtr &p_pCommand, ScriptPage *p_pParent);

private slots:
    void ChangeVariableClicked();
    void CommentClicked();
    void IfConditionClicked();

private:
    ScriptContainer *m_pParent;
};
#endif