#ifndef DATABASE_DIALOG_RESIZE_H
#define DATABASE_DIALOG_RESIZE_H

#include <QtWidgets/QDialog>
#include "UI/UI_DatabaseDialogResize.h"
#include "DatabaseModel.h"

class DatabaseDialogResize : public QDialog, protected Ui_DatabaseDialogResize
{
    Q_OBJECT
public:
    DatabaseDialogResize(IDatabaseModel *pDBModel, QWidget *pParent);

private slots:
    void _onAccept();

private:
    IDatabaseModel *m_pDBModel;
};

#endif
