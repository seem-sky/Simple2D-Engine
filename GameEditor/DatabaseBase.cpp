#include "DatabaseBase.h"
#include "moc_DatabaseBase.h"

DatabaseBase::DatabaseBase(QAbstractItemModel *pModel) : QWidget(), Ui_NewDatabaseWidget()/*, m_pDatabaseView(nullptr), m_pDatabaseSize(nullptr)*/
{
    setupUi(this);
    m_pView->setModel(pModel);

    connect(m_pView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(_onItemClicked(const QModelIndex&)));
    connect(m_pResizeList, SIGNAL(clicked()), this, SLOT(_onDatabaseResize()));

    DATABASE::Database newDatabase;
    newDatabase.setItem(1, new DATABASE::MAP_OBJECT::WorldObjectPrototype(1));
    DATABASE::Prototype *pProto = newDatabase.getItem(1);
}

void DatabaseBase::setupWidgets(const DATABASE::Prototype* pPrototype)
{
    if (!pPrototype)
        return;
    m_pName->setText(pPrototype->getName());
}

DatabaseModel* DatabaseBase::getModel() const
{
    return dynamic_cast<DatabaseModel*>(m_pView->model());
}

// slots
void DatabaseBase::_onDatabaseResize(int size)
{
    if (auto pModel = getModel())
    {
        int32 oldSize = pModel->rowCount(QModelIndex());
        pModel->resize(m_pListCount->value());
        if (oldSize < size)
            pModel->insertRows(oldSize, size-oldSize);
        else
            pModel->removeRows(size, oldSize-size);
    }
}

void DatabaseBase::_onItemClicked(const QModelIndex &index)
{
    //setupWidgets(_getPrototypePtr(index.row()));
}
