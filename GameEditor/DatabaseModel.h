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

        const IDatabase* getDatabase() const
        {
            return m_DBMgr.getDatabase(m_DBType);
        }

        int rowCount(const QModelIndex& parent) const
        {
            return getSize();
        }

        int columnCount(const QModelIndex& parent) const
        {
            return MODEL_COLUMN_COUNT;
        }

        QVariant data(const QModelIndex& index, int role) const
        {
            if(index.isValid())
            {
                switch (role)
                {
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

        QVariant headerData(int section, Qt::Orientation orientation, int role) const
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

        Qt::ItemFlags flags(const QModelIndex& index) const
        {
            if(!index.isValid())
                return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
        }

        bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex())
        {
            beginInsertRows(parent, row, row+count);
            endInsertRows();
            return true;
        }

        bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex())
        {
            beginRemoveRows(parent, row, row+count);
            endRemoveRows();
            return true;
        }

        uint32 getSize() const
        {
            return getDatabase()->getSize();
        }

        uint32 getMaximumSize() const
        {
            return getDatabase()->getMaximumSize();
        }

        const DatabaseMgr& getDatabaseMgr() const { return m_DBMgr; }
        DatabaseType getDatabaseType() const { return m_DBType; }

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

        DatabaseMgr& getDatabaseMgr()
        {
            return const_cast<DatabaseMgr&>(ConstDatabaseModel::getDatabaseMgr());
        }

        IDatabase* getDatabase()
        {
            return getDatabaseMgr().getDatabase(getDatabaseType());
        }

        void resize(uint32 size)
        {
            uint32 uiOldSize = getDatabase()->getSize();
            if (uiOldSize < size)
                insertRows(uiOldSize, size-uiOldSize);
            else if (size < uiOldSize)
                removeRows(size, uiOldSize-size);

            getDatabase()->resize(size);
        }
    };
}

//class IDatabaseModel : public QAbstractListModel
//{
//public:
//    IDatabaseModel() : QAbstractListModel() {}
//
//    virtual void resize(uint32 size) = 0;
//    virtual uint32 getSize() const = 0;
//    virtual uint32 getMaximumSize() const = 0;
//
//    virtual DATABASE::IDatabase* takeDatabase() = 0;
//    virtual DATABASE::IDatabase* getDatabase() = 0;
//};
//
//template <class T>
//class DatabaseModel : public IDatabaseModel
//{
//public:
//    DatabaseModel(std::unique_ptr<T> pDB = std::unique_ptr<T>()) : IDatabaseModel()
//    {
//        setDatabase(std::move(pDB));
//    }
//
//    void setDatabase(std::unique_ptr<T> pDB)
//    {
//        m_pDatabase = std::move(pDB);
//    }
//
//    int rowCount(const QModelIndex& parent) const
//    {
//        if (m_pDatabase)
//            return m_pDatabase->getSize();
//        return 0;
//    }
//
//    int columnCount(const QModelIndex& parent) const
//    {
//        return MODEL_COLUMN_COUNT;
//    }
//
//    QVariant data(const QModelIndex& index, int role) const
//    {
//        if(m_pDatabase && index.isValid())
//        {
//            switch (role)
//            {
//            case Qt::DisplayRole:
//                if (auto pProto = m_pDatabase->getPrototype(index.row()+1))
//                {
//                    switch(index.column())
//                    {
//                    case COLUMN_ID: return pProto->getID();
//                    case COLUMN_NAME: return pProto->getName();
//                    }
//                }
//                break;
//            }
//        }
//        return QVariant();
//    }
//
//    QVariant headerData(int section, Qt::Orientation orientation, int role) const
//    {
//        if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
//        {
//            switch (section)
//            {
//            case COLUMN_ID: return tr("ID");
//            case COLUMN_NAME: return tr("Name");
//            }
//        }
//        return QVariant();
//    }
//
//    Qt::ItemFlags flags(const QModelIndex& index) const
//    {
//        if(!index.isValid())
//            return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
//        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
//    }
//
//    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex())
//    {
//        beginInsertRows(parent, row, row+count);
//        endInsertRows();
//        return true;
//    }
//
//    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex())
//    {
//        beginRemoveRows(parent, row, row+count);
//        endRemoveRows();
//        return true;
//    }
//
//    void resize(uint32 size)
//    {
//        if (!m_pDatabase)
//            return;
//        uint32 uiOldSize = m_pDatabase->getSize();
//        if (uiOldSize < size)
//            insertRows(uiOldSize, size-uiOldSize);
//        else if (size < uiOldSize)
//            removeRows(size, uiOldSize-size);
//
//        m_pDatabase->resize(size);
//    }
//
//    uint32 getSize() const
//    {
//        if (m_pDatabase)
//            return m_pDatabase->getSize();
//        return 0;
//    }
//
//    uint32 getMaximumSize() const
//    {
//        if (m_pDatabase)
//            return m_pDatabase->getMaximumSize();
//        return 0;
//    }
//
//    DATABASE::IDatabase* getDatabase()
//    {
//        return m_pDatabase.get();
//    }
//
//    DATABASE::IDatabase* takeDatabase()
//    {
//        return m_pDatabase.release();
//    }
//
//private:
//    std::unique_ptr<T> m_pDatabase;
//};
//
///*#####
//# special models and typedefs
//#####*/
//class AnimationTypeDatabaseModel : public DatabaseModel<DATABASE::AnimationTypeDatabase>
//{
//public:
//    AnimationTypeDatabaseModel(std::unique_ptr<DATABASE::AnimationTypeDatabase> pDB = std::unique_ptr<DATABASE::AnimationTypeDatabase>()) :
//        DatabaseModel(std::move(pDB))
//    {}
//
//    QVariant data(const QModelIndex& index, int role) const
//    {
//        if(index.isValid() && (role == Qt::ForegroundRole))
//        {
//            if (index.row() <= 8)
//                return QVariant(QColor(Qt::blue));
//            return QVariant(QColor(Qt::black));
//        }
//        else
//            return DatabaseModel::data(index, role);
//    }
//};
//
//typedef DatabaseModel<DATABASE::TileDatabase> TileDatabaseModel;
//typedef DatabaseModel<DATABASE::TileSetDatabase> TileSetDatabaseModel;
//typedef DatabaseModel<DATABASE::AutoTileDatabase> AutoTileDatabaseModel;
//typedef DatabaseModel<DATABASE::SpriteDatabase> SpriteDatabaseModel;
//typedef DatabaseModel<DATABASE::AnimationDatabase> AnimationDatabaseModel;
//typedef DatabaseModel<DATABASE::WorldObjectDatabase> WorldObjectDatabaseModel;
//typedef DatabaseModel<DATABASE::LocalisationDatabase> LocalisationDatabaseModel;

#endif
