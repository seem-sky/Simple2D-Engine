#ifndef VARIABLE_COUNT_DIALOG_H
#define VARIABLE_COUNT_DIALOG_H

#include <QtGui/QDialog>
#include "UI/UI_VariableCountDialog.h"
#include <Global.h>

class VariableCountDialog : public QDialog, Ui_VariableCountDialog
{
    Q_OBJECT
public:
    VariableCountDialog(QWidget *p_pParent, uint32 *p_uiCounter);

private:
    uint32 *m_puiCounter;

private slots:
    void ValueChanged(int p_Value);
};
#endif