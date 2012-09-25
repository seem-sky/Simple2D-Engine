#ifndef CHANGE_VARIABLE_WINDOW_H
#define CHANGE_VARIABLE_WINDOW_H

#include "UI/UI_CommandChangeVariable.h"
#include "EventScriptCommand.h"

class ChangeVariableWindow : public QDialog, public Ui_CommandChangeVariable
{
    Q_OBJECT
public:
    ChangeVariableWindow(EVENT_SCRIPT::CommandChangeVariable *p_pCommand, QWidget *p_pParent = NULL);
    ~ChangeVariableWindow(void);

private slots:
    void AcceptVariable();
    void ChangeVariableType();
    void ChangeOperator();

private:
    EVENT_SCRIPT::CommandChangeVariable *m_pCommand;
};
#endif