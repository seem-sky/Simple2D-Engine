#ifndef DATABASE_WINDOW_H
#define DATABASE_WINDOW_H

#include "UI/UI_Database.h"
#include <QtGui/QCloseEvent>
#include "DatabaseMgr.h"

class DatabaseWindow : public QDialog, Ui_Database
{
    Q_OBJECT
public:
    DatabaseWindow(DATABASE::DatabaseMgr& DBMgr, QWidget* parent);

    QString getCurrentDatabaseSection();
    inline const DATABASE::DatabaseMgr& getDBMgr() const { return m_OwnDBMgr; }

    const DATABASE::DatabaseMgr& getDatabaseMgr() const { return m_OwnDBMgr; }

private slots:
    void clickButtonOK();
    void clickButtonApply();
    void clickButtonCancel();

signals:
    void storeChanges();

private:
    DATABASE::DatabaseMgr m_OwnDBMgr;
    DATABASE::DatabaseMgr& m_DBMgr;
};
#endif