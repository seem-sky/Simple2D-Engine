#ifndef LOCAL_VARIABLE_DIALOG_H
#define LOCAL_VARIABLE_DIALOG_H

#include "ui/UI_LocalVariableDialog.h"
#include <VariableHolder.h>

class LocalVariableDialog : public QDialog, Ui_LocalVariableDialog
{
public:
    LocalVariableDialog(VariableHolder *p_pVariableHolder, QWidget *p_pParent);
};
#endif