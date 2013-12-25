#ifndef DATABASE_MODULE_LIST_H
#define DATABASE_MODULE_LIST_H

#include "UI/UI_DatabaseModuleList.h"
#include "DatabaseModel.h"

class DatabaseModuleList : public QWidget, Ui_DatabaseModuleList
{
    Q_OBJECT
public:
    DatabaseModuleList(QWidget* pParent = nullptr);

    IDatabaseModel* getDatabaseModel() const { return dynamic_cast<IDatabaseModel*>(m_pList->model()); }
    void setDatabaseModel(IDatabaseModel* pModel);

    // data widgets
    inline void setDataName(const QString& name) { m_pName->setText(name); }
    inline QString getDataName() const { return m_pName->text(); }

    inline void setDataID(uint32 uiID) { m_pID->setValue(uiID); }
    inline uint32 getDataID() const { return m_pID->value(); }

    const DATABASE::Prototype* getCurrentPrototype() const;

private slots:
    void _onClickResizeButton();
    void _onSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

signals:
    void selectionChanged();
};

#endif
