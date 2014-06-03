#ifndef DATABASE_MODULE_LIST_H
#define DATABASE_MODULE_LIST_H

#include "UI/UI_DatabaseModuleList.h"
#include "DatabaseModel.h"

class DatabaseModuleList : public QWidget, Ui_DatabaseModuleList
{
    Q_OBJECT
public:
    DatabaseModuleList(QWidget* pParent = nullptr);

    DATABASE::DatabaseModel* getDatabaseModel() const { return dynamic_cast<DATABASE::DatabaseModel*>(m_pList->model()); }
    void setDatabaseModel(DATABASE::DatabaseModel* pModel);

    // data widgets
    inline void setDataName(const QString& name) { m_pName->setText(name); }
    inline QString getDataName() const { return m_pName->text(); }

    inline void setDataID(uint32 uiID) { m_pID->setValue(uiID); }
    inline uint32 getDataID() const { return m_pID->value(); }

    const DATABASE::PROTOTYPE::Prototype* getCurrentPrototype() const;

    void selectItem(uint32 ID);

private slots:
    void _onClickResizeButton();
    void _onSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

signals:
    void selectionChanged();
};

#endif
