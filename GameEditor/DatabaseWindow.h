#ifndef DATABASE_WINDOW_H
#define DATABASE_WINDOW_H

#include "UI/UI_Database.h"
#include "ModifyObject.h"
#include <QtGui/QCloseEvent>
#include "DatabaseMgr.h"

class DatabaseWindow : public QDialog, Ui_Database
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *p_pEvent) { m_ModifyObj.resizeEvent(this); }

private:
    void saveDatabase();
    void clearWidgets();

public:
    DatabaseWindow(const DATABASE::DatabaseMgr &DBMgr, QWidget *parent);

    QString getCurrentDatabaseSection();
    inline const DATABASE::DatabaseMgr& getDBMgr() const { return m_pDBMgr; }

private slots:
    void clickButtonOK();
    void clickButtonApply();
    void clickButtonCancel();
    void _focusChanged(int index);

private:
    ModifyObject m_ModifyObj;
    const DATABASE::DatabaseMgr &m_pDBMgr;
};
#endif