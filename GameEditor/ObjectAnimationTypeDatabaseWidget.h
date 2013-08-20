#ifndef OBJECT_ANIMATION_DATABASE_WIDGET_H
#define OBJECT_ANIMATION_DATABASE_WIDGET_H

#include "DatabaseWidget.h"

class ObjectAnimationTypeDatabaseWidget : public DatabaseWidget<DATABASE::ObjectAnimationTypePrototype>
{
    Q_OBJECT
protected:
    bool setWidgetsFromPrototype(const DATABASE::ObjectAnimationTypePrototypePtr &proto);
    void resizeDatabase(uint32 uiSize);
    void fillListWidget(const UInt32StringMap &sStrings);

public:
    ObjectAnimationTypeDatabaseWidget(QWidget *pParent = NULL);

private slots:
    DATABASE_WIDGET_SLOTS
};
#endif