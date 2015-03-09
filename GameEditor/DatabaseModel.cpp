#include "DatabaseModel.h"
#include <QtGui/QColor>

using namespace database;

Model::Model(const Interface& db, QObject* pParent)
    : m_DB(db), QAbstractListModel(pParent)
{}

int Model::rowCount(const QModelIndex& parent) const
{
    return m_DB.getSize();
}

enum ColumnName
{
    ID,
    name
};
const uint32 MODEL_COLUMN_COUNT = 2;

int Model::columnCount(const QModelIndex& parent) const
{
    return MODEL_COLUMN_COUNT;
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    if (index.isValid())
    {
        switch (role)
        {
            // draw standard entries blue
        case Qt::ForegroundRole:
            // ToDo: standard entries
            /*if (getDatabaseType() == DatabaseType::ANIMATION_TYPE_DATABASE && index.row() < PROTOTYPE::ANIMATION::STANDARD_ANIMATION_TYPES)
                return QVariant(QColor(Qt::blue));*/
            return QVariant(QColor(Qt::black));

        case Qt::DisplayRole:
            switch (index.column())
            {
            case ID: return index.row() + 1;
            case name:
                if (auto pProto = m_DB.getPrototype(index.row() + 1))
                    return pProto->getName();
            }
            break;
        }
    }
    return QVariant();
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (static_cast<ColumnName>(section))
        {
        case ID: return tr("ID");
        case name: return tr("Name");
        }
    }
    return QVariant();
}

Qt::ItemFlags Model::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
}

bool Model::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count);
    endInsertRows();
    return true;
}

bool Model::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count);
    endRemoveRows();
    return true;
}
