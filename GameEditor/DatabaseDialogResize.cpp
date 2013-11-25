#include "DatabaseDialogResize.h"
#include "moc_DatabaseDialogResize.h"
#include <QtWidgets/QMessageBox>

DatabaseDialogResize::DatabaseDialogResize(IDatabaseModel *pDBModel, QWidget *pParent) : QDialog(pParent), Ui_DatabaseDialogResize(), m_pDBModel(pDBModel)
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
    if (m_pNewSize->value() != m_pOldSize->value())
    {
        if (m_pNewSize->value() < m_pOldSize->value())
        {
            if (QMessageBox::Yes != QMessageBox::question(0, "Delete " + QString::number(m_pOldSize->value() - m_pNewSize->value()) + " entrys.",
                "Do you really want to delete entry " + QString::number(m_pNewSize->value()+1) + " up to end?", QMessageBox::Yes | QMessageBox::No))
                return;
        }

        m_pDBModel->resize(m_pNewSize->value());
    }
}