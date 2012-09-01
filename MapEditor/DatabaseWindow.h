#ifndef DATABASE_WINDOW_H
#define DATABASE_WINDOW_H

#include "UI/UI_Database.h"
#include "ResizeWidget.h"
#include <QtGui/QCloseEvent>
#include "DatabaseOutput.h"

class DatabaseWindow : public QMainWindow, Ui_Database
{
    Q_OBJECT
public:
    DatabaseWindow(QMainWindow *parent);
    ~DatabaseWindow(void);

    std::string GetCurrentDatabaseSection();

private slots:
    void ClickButtonOK();
    void ClickButtonApply();

protected:
    void closeEvent(QCloseEvent *p_pEvent);
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.ResizeEvent(this); }

private:
    void ClearWidgets();
    std::string m_sLogLocationName;
    ResizeWidget m_ResizeObj;
};
#endif