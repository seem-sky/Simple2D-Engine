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
    void _resizeButtonClicked() { resizeDatabase(getListCountValue()); }
    void _currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) { currentItemChanged(current); }
    void _changeName() { changeName(); }
};
#endif