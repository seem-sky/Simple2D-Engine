#include "LocalVariableDialog.h"

LocalVariableDialog::LocalVariableDialog(VariableHolder *p_pVariableHolder, QWidget *p_pParent) : QDialog(p_pParent), Ui_LocalVariableDialog()
{
    setupUi(this);
    if (!p_pVariableHolder)
        return;

    m_pBools->SetVariableHolder(p_pVariableHolder);
    m_pInts->SetVariableHolder(p_pVariableHolder);
    m_pFloats->SetVariableHolder(p_pVariableHolder);
    m_pStrings->SetVariableHolder(p_pVariableHolder);
}