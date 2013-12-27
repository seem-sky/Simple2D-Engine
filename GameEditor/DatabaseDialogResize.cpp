#include "DatabaseDialogResize.h"
#include "moc_DatabaseDialogResize.h"
#include <QtWidgets/QMessageBox>

DatabaseDialogResize::DatabaseDialogResize(DATABASE::DatabaseModel* pDBModel, QWidget* pParent) : QDialog(pParent), Ui_DatabaseDialogResize(), m_pDBModel(pDBModel)
{
    setupUi(this);

    // setup fields
    m_pOldSize->setValue(m_pDBModel->getSize());
    m_pMaxSize->setValue(m_pDBModel->getMaximumSize());
    m_pNewSize->setValue(m_pDBModel->getSize());
    m_pNewSize->setMaximum(m_pDBModel->getMaximumSize());

    connect(m_pButtons, SIGNAL(accepted()), this, SLOT(_onAccept()));
}

void DatabaseDialogResize::_onAccept()
{
    uint32 oldSize = m_pOldSize->value(), newSize = m_pNewSize->value();
    if (newSize != oldSize)
    {
        if (newSize < oldSize)
        {
            if (QMessageBox::Yes != QMessageBox::question(0, "Delete " + QString::number(oldSize - newSize) + " entries.",
                "Do you really want to delete entry " + QString::number(newSize+1) + " up to end?", QMessageBox::Yes | QMessageBox::No))
                return;
        }

        m_pDBModel->resize(newSize);
    }
}