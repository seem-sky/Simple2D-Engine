#include "DatabaseModuleList.h"
#include "moc_DatabaseModuleList.h"
#include "DatabaseDialogResize.h"

DatabaseModuleList::DatabaseModuleList(QWidget *pParent) : QWidget(pParent), Ui_DatabaseModuleList()
{
    setupUi(this);
    
    //setup list
    m_pList->setIndentation(0);

    connect(m_pResizeButton, SIGNAL(clicked()), this, SLOT(_onClickResizeButton()));
}

void DatabaseModuleList::_onClickResizeButton()
{
    DatabaseDialogResize resizeDialog(getDatabaseModel(), this);
    resizeDialog.exec();
}

void DatabaseModuleList::_onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit selectionChanged();
}

const DATABASE::Prototype* DatabaseModuleList::getCurrentPrototype() const
{
    // row+1 == prototype ID
    return getDatabaseModel()->getDatabase()->getPrototype(m_pList->currentIndex().row()+1);
}

void DatabaseModuleList::setDatabaseModel(IDatabaseModel *pModel)
{
    m_pList->setModel(pModel);
    connect(m_pList->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
        this, SLOT(_onSelectionChanged(const QModelIndex&, const QModelIndex&)));
    m_pList->resizeColumnToContents(0);
    m_pList->setCurrentIndex(pModel->index(0));
}