#ifndef DATABASE_WINDOW_H
#define DATABASE_WINDOW_H

#include "UI/UI_Database.h"
#include "ModifyObject.h"
#include <QtGui/QCloseEvent>
#include "DatabaseOutput.h"

class DatabaseWindow : public QDialog, Ui_Database
{
    Q_OBJECT
public:
    DatabaseWindow(QWidget *parent);

    std::string GetCurrentDatabaseSection();

private slots:
    void ClickButtonOK();
    void ClickButtonApply();
    void DBSaved();

signals:
    void ReloadPage();

protected:
    void closeEvent(QCloseEvent *p_pEvent);
    void resizeEvent(QResizeEvent *p_pEvent) { m_ModifyObj.resizeEvent(this); }

private:
    void SaveDatabase();
    void ClearWidgets();
    std::string m_sLogLocationName;
    ModifyObject m_ModifyObj;
};
#endif