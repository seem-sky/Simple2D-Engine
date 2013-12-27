#ifndef DATABASE_WIDGET_BASE_H
#define DATABASE_WIDGET_BASE_H

#include "DatabaseModuleList.h"
#include "Database.h"
#include "DatabaseModel.h"

class DatabaseWidgetBase : public QWidget
{
    Q_OBJECT
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype);
    virtual void clear();

    void hideEvent(QHideEvent* pEvent);

public:
    DatabaseWidgetBase(QWidget* pParent = nullptr);

    virtual void setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr) = 0;
    DATABASE::ConstDatabaseModel* getDatabaseModel() const;

private slots:
    void _onItemSelected();

public slots:
    void saveCurrent();

protected:
    DatabaseModuleList* m_pModuleList;
};

#endif
