#ifndef DATABASE_MODEL_H
#define DATABASE_MODEL_H

#include <QtCore/QAbstractListModel>
#include "DatabaseMgr.h"

namespace DATABASE
{
    enum class ColumnName
    {
        COLUMN_ID,
        COLUMN_NAME
    };
    const uint32 MODEL_COLUMN_COUNT = 2;

    class ConstDatabaseModel : public QAbstractListModel
    {
    public:
        ConstDatabaseModel(const DatabaseMgr& DBMgr, DatabaseType type) : QAbstractListModel(), m_DBType(type), m_DBMgr(DBMgr)
        {
        }

        const IDatabase* getDatabase() const;

        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        uint32 getSize() const;
        uint32 getMaximumSize() const;

        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        Qt::ItemFlags flags(const QModelIndex& index) const;

        bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
        bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

        const DatabaseMgr& getDatabaseMgr() const;
        DatabaseType getDatabaseType() const;

    private:
        DatabaseType m_DBType;
        const DatabaseMgr& m_DBMgr;
    };

    class DatabaseModel : public ConstDatabaseModel
    {
    public:
        DatabaseModel(DatabaseMgr& DBMgr, DatabaseType type) : ConstDatabaseModel(DBMgr, type)
        {
        }

        DatabaseMgr& getDatabaseMgr();
        IDatabase* getDatabase();

        void resize(uint32 size);
    };
}

#endif
