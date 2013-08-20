#ifndef WORLD_OBJECT_DATABASE_WIDGET_H
#define WORLD_OBJECT_DATABASE_WIDGET_H

#include "ObjectDatabaseWidget.h"

/*#####
# WorldObjectDatabaseWidget
#####*/
class WorldObjectDatabaseWidget : public ObjectDatabaseWidget<DATABASE::MAP_OBJECT::WorldObjectPrototype>
{
    Q_OBJECT
public:
    WorldObjectDatabaseWidget(QWidget *pParent = NULL);

private slots:
    OBJECT_DATABASE_WIDGET_SLOTS
};
#endif