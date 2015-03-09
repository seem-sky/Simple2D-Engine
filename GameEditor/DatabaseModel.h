#pragma once

#include <QtCore/QAbstractListModel>
#include <vector>
#include <Database/Interface.h>

namespace database
{
    class Model : public QAbstractListModel
    {
    public:
        Model(const Interface& db, QObject* pParent = nullptr);

        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;

        QVariant data(const QModelIndex& index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        Qt::ItemFlags flags(const QModelIndex& index) const;

        bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
        bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

    private:
        const Interface& m_DB;
    };
}
