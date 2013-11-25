#ifndef DATABASE_BASE_H
#define DATABASE_BASE_H

#include <QtWidgets/QTreeView>
#include <QtWidgets/QSpinBox>
#include "DatabaseModel.h"
#include "UI/UI_NewDatabaseWidget.h"

class DatabaseBase : public QWidget, public Ui_NewDatabaseWidget
{
    Q_OBJECT
protected:
    virtual void setupWidgets(const DATABASE::Prototype *pPrototype);

public:
    DatabaseBase(QAbstractItemModel *pModel);

    virtual DatabaseModel* getModel() const;

private slots:
    void _onDatabaseResize(int size);
    void _onItemClicked(const QModelIndex &index);

//private:
//    QTreeView *m_pDatabaseView;
//    QSpinBox *m_pDatabaseSize;
};
#endif
