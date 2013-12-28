#ifndef DATABASE_WIDGET_RESOURCE_H
#define DATABASE_WIDGET_RESOURCE_H

#include "DatabaseWidgetBase.h"
#include <QtWidgets/QPushButton>
#include "DatabaseModuleResource.h"

class DatabaseWidgetRessource : public DatabaseWidgetBase
{
    Q_OBJECT
private:
    virtual QString _selectFile() const = 0;
    virtual QStringList _selectFiles() const = 0;
    void _import(uint32 uiID, const QString& fileNamePath);

protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype);
    virtual void clear();

public:
    DatabaseWidgetRessource(QWidget* pParent = nullptr);

protected:
    QPushButton* m_pMassImport;
    DatabaseModuleResource* m_pModuleResource;

private slots:
    void _onClickFileButton();
    void _onClickMassFileImportButton();

signals:
    void fileImport(uint32 ID);
};

#endif
