#include "DatabaseModuleLocalisation.h"
#include <QtWidgets/QPlainTextEdit>

using namespace DATABASE::PROTOTYPE::LOCALISATION;

DatabaseModuleLocalisation::DatabaseModuleLocalisation(QWidget* pParent) : QWidget(pParent), Ui_DatabaseModuleLocalisation()
{
    setupUi(this);

    // setup table
    m_pLocalTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _setRowCount(LOCALISATION_COUNT);
}

void DatabaseModuleLocalisation::_setRowCount(uint32 count)
{
    const uint32 oldCount = m_pLocalTable->rowCount();
    m_pLocalTable->setRowCount(count);
    const uint32 height = (m_pLocalTable->rowHeight(0)+m_pLocalTable->lineWidth())*count;
    m_pLocalTable->setMinimumHeight(height);
    m_pLocalTable->setMaximumHeight(height);

    for (uint32 i = oldCount; i < count; ++i)
    {
        m_pLocalTable->setCellWidget(i, 0, new QPlainTextEdit());

    }
}

QString DatabaseModuleLocalisation::getLocalisation(uint32 index)
{
    QString result;
    if (auto pTextEdit = dynamic_cast<QPlainTextEdit*>(m_pLocalTable->cellWidget(index, 0)))
        result = pTextEdit->toPlainText();
    return result;
}

void DatabaseModuleLocalisation::setLocalisation(uint32 index, const QString& localisation)
{
    if (auto pTextEdit = dynamic_cast<QPlainTextEdit*>(m_pLocalTable->cellWidget(index, 0)))
        pTextEdit->setPlainText(localisation);
}

void DatabaseModuleLocalisation::clear()
{
    for (int row = 0; row < m_pLocalTable->rowCount(); ++row)
    {
        if (auto pTextEdit = dynamic_cast<QPlainTextEdit*>(m_pLocalTable->cellWidget(row, 0)))
            pTextEdit->clear();
    }
}
