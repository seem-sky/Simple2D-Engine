#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "UI/UI_MainWindow.h"
#include "ModifyObject.h"
#include "Point.h"
#include "MapDatabase.h"
#include "Config.h"
#include "Project.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
private slots:
    void _openDatabase();
    void _newProject();
    void _loadProject();
    void _saveProject();
    void _closeProject();
    void _mapScreenshot();

signals:
    void projectLoadDone();
    void saveProject();

private:
    bool _loadDB(const QString &projectPath);
    bool _loadProject(const QString &sDir);
    void _setDBs();

protected:
    void resizeEvent(QResizeEvent *pEvent);
    void moveEvent(QMoveEvent *pEvent) { Config::Get()->setMainWindowPos(Point<uint32>(pEvent->pos().x(), pEvent->pos().y())); }

public:
    MainWindow(QMainWindow *parent = 0);
    ~MainWindow(void);

    inline MapEditorWidget* getMapEditorWidget() const { return m_pMapEditor; }

    inline DATABASE::ConstDatabaseMgrPtr getDatabaseMgr() const { return m_project.getDatabaseMgr(); }

private:
    ModifyObject m_ModifyObj;
    Project m_project;
};
#endif