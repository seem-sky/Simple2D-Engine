#include "IndexDialog.h"

IndexDialog::IndexDialog(uint32 p_uiIndex) : QDialog(), Ui_IndexDialog()
{
    setupUi(this);
    m_pIndex->setMaximum(p_uiIndex);
}
