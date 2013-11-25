#ifndef DATABASE_WIDGET_BASE_H
#define DATABASE_WIDGET_BASE_H

#include "DatabaseModuleList.h"
#include "Database.h"
#include "DatabaseModel.h"

class DatabaseWidgetBase : public QWidget
{
    Q_OBJECT
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::Prototype *pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::Prototype *pPrototype);
    virtual void clear();

public:
    DatabaseWidgetBase(QWidget *pParent = nullptr);

    inline DATABASE::IDatabase* getDatabase() const { return m_pModuleList->getDatabaseModel()->getDatabase(); }
    inline IDatabaseModel* getDatabaseModel() const { return m_pModuleList->getDatabaseModel(); }
    inline void setDatabaseModel(IDatabaseModel *pModel) { m_pModuleList->setDatabaseModel(pModel); }

    inline DATABASE::IDatabasePtr takeDatabase() { return m_pModuleList->getDatabaseModel()->takeDatabase(); }

private slots:
    void _onItemSelected();

protected:
    DatabaseModuleList *m_pModuleList;
};

#endif
