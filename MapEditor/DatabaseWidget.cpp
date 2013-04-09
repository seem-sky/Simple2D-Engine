#include "DatabaseWidget.h"

using namespace DATABASE;

/*#####
# OrderedTreeWidgetItem
#####*/
PrototypeTreeWidgetItem::PrototypeTreeWidgetItem(const QStringList &strings) : QTreeWidgetItem(strings) {}

bool PrototypeTreeWidgetItem::operator <(const QTreeWidgetItem &other) const  
{
    switch (treeWidget()->sortColumn())
    {
    case 0: return text(0).toUInt() < other.text(0).toUInt();
    default: return QTreeWidgetItem::operator <(other);
    }
}