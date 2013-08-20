#include "WorldObjectDatabaseWidget.h"
#include "moc_WorldObjectDatabaseWidget.h"

using namespace DATABASE;

/*#####
# WorldObjectDatabaseWidget
#####*/
WorldObjectDatabaseWidget::WorldObjectDatabaseWidget(QWidget *pParent) : ObjectDatabaseWidget(pParent)
{
    connectWidgets();
}