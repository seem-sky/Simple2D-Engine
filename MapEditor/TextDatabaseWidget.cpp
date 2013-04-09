#include "TextDatabaseWidget.h"
#include "moc_TextDatabaseWidget.h"
#include <QtGui/QTextEdit>

using namespace DATABASE;

const uint32 LOCAL_TABLE_CELL_HEIGHT = 100;

TextDatabaseWidget::TextDatabaseWidget(QWidget *pParent) : DatabaseWidget(pParent), Ui_TextDatabaseWidget()
{
    Ui_TextDatabaseWidget::setupUi(this);
    connectWidgets();
}

void TextDatabaseWidget::_localChanged()
{
    updateItem();
}

void TextDatabaseWidget::clearWidgets()
{
    DatabaseWidget::clearWidgets();
    m_pLocalTable->setRowCount(1);
    m_pLocalTable->resize(m_pLocalTable->width(), LOCAL_TABLE_CELL_HEIGHT+2);
    _addCellTextEdit(QPoint(0,0));
}

bool TextDatabaseWidget::getPrototypeFromWidgets(TextPrototypePtr &proto)
{
    if (!DatabaseWidget::getPrototypeFromWidgets(proto))
        return false;

    proto->setLocalCount((uint32)m_pLocalTable->rowCount());
    for (int i = 0; i < m_pLocalTable->rowCount(); ++i)
    {
        // set locals
        if (QTextEdit *pItem = (QTextEdit*)m_pLocalTable->cellWidget(i, 0))
            proto->setLocal(i, pItem->toPlainText().toStdString());
    }
    return true;
}

bool TextDatabaseWidget::setWidgetsFromPrototype(const TextPrototypePtr &proto)
{
    if (!DatabaseWidget::setWidgetsFromPrototype(proto))
        return false;

    m_pLocalTable->setRowCount(proto->getLocalCount() == 0 ? 1 : proto->getLocalCount());
    m_pLocalTable->resize(m_pLocalTable->width(), proto->getLocalCount()*LOCAL_TABLE_CELL_HEIGHT+2);
    for (uint32 i = 0; i < proto->getLocalCount(); ++i)
    {
        // if not the first header
        if (i)
        {
            if (QTableWidgetItem *pItem = m_pLocalTable->verticalHeaderItem(i))
                pItem->setText("local " + QString::number(i));
        }
        _addCellTextEdit(QPoint(0, i), QString::fromStdString(proto->getLocal(i)));
    }
    return true;
}

void TextDatabaseWidget::_addCellTextEdit(QPoint pos, QString sText)
{
    QTextEdit *pEdit = new QTextEdit();
    pEdit->setPlainText(sText);
    m_pLocalTable->setCellWidget(pos.y(), pos.x(), pEdit);
    connect(pEdit, SIGNAL(textChanged()), this, SLOT(_localChanged()));
}
