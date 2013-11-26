#ifndef DATABASE_WIDGET_LOCALISATION_H
#define DATABASE_WIDGET_LOCALISATION_H

#include "DatabaseWidgetBase.h"
#include "DatabaseModuleLocalisation.h"

class DatabaseWidgetLocalisation : public DatabaseWidgetBase
{
protected:
    void setupWidgetsFromPrototype(const DATABASE::Prototype *pPrototype);
    void setupPrototypeFromWidgets(DATABASE::Prototype *pPrototype);

public:
    DatabaseWidgetLocalisation(QWidget *pParent = nullptr);

    void clear();

private:
    DatabaseModuleLocalisation *m_pModuleLocalisation;
};

#endif
