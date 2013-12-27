#include "DatabaseModel.h"

using namespace DATABASE;

const IDatabase* ConstDatabaseModel::getDatabase() const
{
    return m_DBMgr.getDatabase(m_DBType);
}

int ConstDatabaseModel::rowCount(const QModelIndex& parent) const
{
    return getSize();
}

int ConstDatabaseModel::columnCount(const QModelIndex& parent) const
{
    return MODEL_COLUMN_COUNT;
}

QVariant ConstDatabaseModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid())
    {
        switch (role)
        {
            // draw standard entry ins AnimationTypeDatabase blue
        case Qt::ForegroundRole:
            if (getDatabaseType() == DatabaseType::ANIMATION_TYPE_DATABASE && index.row() <= 8)
                return QVariant(QColor(Qt::blue));
            return QVariant(QColor(Qt::black));

        case Qt::DisplayRole:
            if (auto pProto = getDatabase()->getPrototype(index.row()+1))
            {
                switch(index.column())
                {
                case ColumnName::COLUMN_ID: return pProto->getID();
                case ColumnName::COLUMN_NAME: return pProto->getName();
                }
            }
            break;
        }
    }
    return QVariant();
}

QVariant ConstDatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case ColumnName::COLUMN_ID: return tr("ID");
        case ColumnName::COLUMN_NAME: return tr("Name");
        }
    }
    return QVariant();
}

Qt::ItemFlags ConstDatabaseModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
}

bool ConstDatabaseModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row+count);
    endInsertRows();
    return true;
}

bool ConstDatabaseModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row+count);
    endRemoveRows();
    return true;
}

uint32 ConstDatabaseModel::getSize() const
{
    return getDatabase()->getSize();
}

uint32 ConstDatabaseModel::getMaximumSize() const
{
    return getDatabase()->getMaximumSize();
}

const DatabaseMgr& ConstDatabaseModel::getDatabaseMgr() const
{
    return m_DBMgr;
}

DatabaseType ConstDatabaseModel::getDatabaseType() const
{
    return m_DBType;
}

DatabaseMgr& DatabaseModel::getDatabaseMgr()
{
    return const_cast<DatabaseMgr&>(ConstDatabaseModel::getDatabaseMgr());
}

IDatabase* DatabaseModel::getDatabase()
{
    return getDatabaseMgr().getDatabase(getDatabaseType());
}

void DatabaseModel::resize(uint32 size)
{
    uint32 uiOldSize = getDatabase()->getSize();
    if (uiOldSize < size)
        insertRows(uiOldSize, size-uiOldSize);
    else if (size < uiOldSize)
        removeRows(size, uiOldSize-size);

    getDatabase()->resize(size);
}
