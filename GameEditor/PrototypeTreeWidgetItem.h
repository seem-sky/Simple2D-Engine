#ifndef PROTOTYPE_TREE_WIDGET_ITEM_H
#define PROTOTYPE_TREE_WIDGET_ITEM_H

#include <QtWidgets/QTreeWidgetItem>

class PrototypeTreeWidgetItem : public QTreeWidgetItem
{
public:
    PrototypeTreeWidgetItem(const QStringList &strings) : QTreeWidgetItem(strings)
    {}

    bool operator <(const QTreeWidgetItem &other) const
    {
        switch (treeWidget()->sortColumn())
        {
        case 0: return text(0).toUInt() < other.text(0).toUInt();
        default: return QTreeWidgetItem::operator <(other);
        }
    }
};

#endif