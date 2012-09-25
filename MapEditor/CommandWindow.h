#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include "UI/UI_CommandDialog.h"

class CommandWindow : public QDialog, public Ui_CommandDialog
{
    Q_OBJECT
public:
    CommandWindow(QWidget *p_pParent = NULL);
    ~CommandWindow(void);

private slots:
    void ChangeVariableClicked();
};
#endif