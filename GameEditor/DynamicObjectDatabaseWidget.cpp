#include "DynamicObjectDatabaseWidget.h"
#include "moc_DynamicObjectDatabaseWidget.h"

using namespace DATABASE;

/*#####
# DynamicObjectDatabaseWidget
#####*/
DynamicObjectDatabaseWidget::DynamicObjectDatabaseWidget(QWidget *pParent) : ObjectDatabaseWidget(pParent)
{
    connectWidgets();
}