#ifndef DATABASE_WINDOW_H
#define DATABASE_WINDOW_H

#include "UI/UI_Database.h"
#include <QtGui/QCloseEvent>
#include "DatabaseMgr.h"

class DatabaseWindow : public QDialog, Ui_Database
{
    Q_OBJECT
private:
    void saveDatabase();
    void clearWidgets();

public:
    DatabaseWindow(DATABASE::DatabaseMgr &DBMgr, QWidget *parent);

    QString getCurrentDatabaseSection();
    inline const DATABASE::DatabaseMgr& getDBMgr() const { return m_pDBMgr; }

private slots:
    void clickButtonOK();
    void clickButtonApply();
    void clickButtonCancel();

signals:
    void storeChanges();

private:
    DATABASE::DatabaseMgr &m_pDBMgr;
};
#endif