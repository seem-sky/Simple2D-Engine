#ifndef DYNAMIC_OBJECT_DATABASE_WIDGET_H
#define DYNAMIC_OBJECT_DATABASE_WIDGET_H

#include "ObjectDatabaseWidget.h"

/*#####
# DynamicObjectDatabaseWidget
#####*/
class DynamicObjectDatabaseWidget : public ObjectDatabaseWidget<DATABASE::MAP_OBJECT::DynamicObjectPrototype>
{
    Q_OBJECT
public:
    DynamicObjectDatabaseWidget(QWidget *pParent = NULL);

private slots:
    OBJECT_DATABASE_WIDGET_SLOTS
};
#endif