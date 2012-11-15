#include "VariableCountDialog.h"
#include "moc_VariableCountDialog.h"

VariableCountDialog::VariableCountDialog(QWidget *p_pParent, uint32 *p_uiCounter) : QDialog(p_pParent), Ui_VariableCountDialog(), m_puiCounter(p_uiCounter)
{
    setupUi(this);

    connect(m_pVariableCount, SIGNAL(valueChanged(int)), this, SLOT(ValueChanged(int)));

    if (m_puiCounter)
        m_pVariableCount->setValue(*m_puiCounter);
}

void VariableCountDialog::ValueChanged(int p_Value)
{
    if (p_Value < 0)
        return;

    *m_puiCounter = (uint32)p_Value;
}