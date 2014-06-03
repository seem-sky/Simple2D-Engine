#include "DatabaseModuleList.h"
#include "moc_DatabaseModuleList.h"
#include "DatabaseDialogResize.h"
#include <QtWidgets/QMessageBox>

using namespace DATABASE::PROTOTYPE;

DatabaseModuleList::DatabaseModuleList(QWidget* pParent) : QWidget(pParent), Ui_DatabaseModuleList()
{
    setupUi(this);
    
    //setup list
    m_pList->setIndentation(0);

    connect(m_pResizeButton, SIGNAL(clicked()), this, SLOT(_onClickResizeButton()));
}

void DatabaseModuleList::_onClickResizeButton()
{
    try
    {
        DatabaseDialogResize resizeDialog(getDatabaseModel(), this);
        resizeDialog.exec();
    }
    catch (const std::bad_alloc& e)
    {
        QMessageBox::information(0, "Resize error.", QString("I'm sorry, but those are too many elements.\n") +
            QString("The Error-Message is: ") + QString(e.what()), QMessageBox::Ok, QMessageBox::Ok);
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::information(0, "Resize error.", e.what(), QMessageBox::Ok, QMessageBox::Ok);
    }
}

void DatabaseModuleList::_onSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
    emit selectionChanged();
}

const Prototype* DatabaseModuleList::getCurrentPrototype() const
{
    // row+1 == prototype ID
    return getDatabaseModel()->getDatabase()->getPrototype(m_pList->currentIndex().row()+1);
}

void DatabaseModuleList::setDatabaseModel(DATABASE::DatabaseModel* pModel)
{
    m_pList->setModel(pModel);
    connect(m_pList->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
        this, SLOT(_onSelectionChanged(const QModelIndex&, const QModelIndex&)));
    m_pList->resizeColumnToContents(0);
    m_pList->setCurrentIndex(pModel->index(0));
}

void DatabaseModuleList::selectItem(uint32 ID)
{
    auto model = m_pList->model()->index(ID, 0);
    if (model.isValid())
        m_pList->setCurrentIndex(model);
}
