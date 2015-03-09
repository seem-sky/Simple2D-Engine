#include "DatabaseModuleResize.h"
#include "moc_DatabaseModuleResize.h"
#include <Database/Interface.h>

using namespace database;
using namespace ui::module;

/*#####
# ResizeButton
#####*/
ResizeButton::ResizeButton(Interface& db, QWidget* pParent)
    : QPushButton(pParent), m_DB(db)
{
    setText("resize");
    connect(this, SIGNAL(clicked()), this, SLOT(_clicked()));
}

void ResizeButton::_clicked()
{
    ResizeDialog dialog(m_DB.getSize(), m_DB.getMaximumSize(), this);
    if (dialog.exec() == QDialog::Accepted)
    {
        auto oldSize = m_DB.getSize();
        m_DB.setSize(dialog.getNewSize());
        emit sizeChanged(dialog.getNewSize(), oldSize);
    }
}

/*#####
# ResizeDialog
#####*/

ResizeDialog::ResizeDialog(uint32 curSize, uint32 maxSize, QWidget* pParent)
    : QDialog(pParent), Ui_DatabaseDialogResize()
{
    setupUi(this);

    m_pNewSize->setMaximum(maxSize);
    m_pNewSize->setValue(curSize);

    m_pOldSize->setValue(curSize);

    m_pMaxSize->setValue(maxSize);
}

uint32 ResizeDialog::getNewSize() const
{
    return m_pNewSize->value();
}
