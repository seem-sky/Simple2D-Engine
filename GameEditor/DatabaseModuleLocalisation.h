#ifndef DATABASE_MODULE_LOCALISATION_H
#define DATABASE_MODULE_LOCALISATION_H

#include "UI/UI_DatabaseModuleLocalisation.h"
#include "Database.h"

class DatabaseModuleLocalisation : public QWidget, Ui_DatabaseModuleLocalisation
{
private:
    void _setRowCount(uint32 count);

public:
    DatabaseModuleLocalisation(QWidget* pParent = nullptr);

    void setLocalisation(uint32 index, const QString& localisation);
    QString getLocalisation(uint32 index);

    void clear();
};

#endif
