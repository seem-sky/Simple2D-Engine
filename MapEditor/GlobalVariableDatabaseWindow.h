#ifndef GLOBAL_VARIABLE_DATABASE_WINDOW_H
#define GLOBAL_VARIABLE_DATABASE_WINDOW_H

#include "UI/UI_GlobalVariableDatabase.h"

class GlobalVariableDatabaseWindow : public QWidget, public Ui_GlobalVariableDatabase
{
    Q_OBJECT
public:
    GlobalVariableDatabaseWindow(QWidget *p_pParent = NULL);

private slots:
    void LoadPage();
};
#endif