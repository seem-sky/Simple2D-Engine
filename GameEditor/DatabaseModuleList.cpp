#include "DatabaseModuleList.h"
#include "moc_DatabaseModuleList.h"
#include "DatabaseModel.h"

using namespace database;
using namespace ui::module;

List::List(Model* pModel, QWidget* pParent)
    : List(pParent)
{
    setup(pModel);
}

List::List(const Interface& DB, QWidget* pParent)
    : List(pParent)
{
    setup(DB);
}

List::List(QWidget* pParent)
    : QTreeView(pParent)
{
    setIndentation(0);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

void List::setup(Model* pModel)
{
    setModel(pModel);
    resizeColumnToContents(0);
   
    connect(selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
        SLOT(_onSelectionChanged(const QModelIndex&, const QModelIndex&)));
}

void List::setup(const Interface& DB)
{
    setup(new database::Model(DB, parent()));
}

uint32 List::getCurrentID() const
{
    return currentIndex().row() + 1;
}

void List::_onSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
    emit selectionChanged(current.isValid() ? current.row() + 1 : 0,
        previous.isValid() ? previous.row() + 1 : 0);
}

void List::onSizeChanged(uint32 newSize, uint32 oldSize)
{
    if (newSize > oldSize)
        model()->insertRows(oldSize, newSize - oldSize);
    else if (oldSize > newSize)
        model()->removeRows(newSize, oldSize - newSize);
    if (!oldSize)
        setCurrentIndex(model()->index(0, 0));
}

void List::selectPrototype(uint32 ID)
{
    auto item = model()->index(ID, 0);
    if (item.isValid())
        setCurrentIndex(item);
}
