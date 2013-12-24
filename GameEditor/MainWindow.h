#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "UI/UI_MainWindow.h"
#include "Point.h"
#include "MapDatabase.h"
#include "Config.h"
#include "Project.h"
#include <QtGui/QMoveEvent>
#include "MapEditorWidgetEditor.h"

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
    // void moveEvent(QMoveEvent *pEvent) { Config::get()->setMainWindowPos(UInt32Point(pEvent->pos().x(), pEvent->pos().y())); }

public:
    MainWindow(QMainWindow *parent = 0);
    ~MainWindow(void);

    //inline MapEditorWidget* getMapEditorWidget() const { return m_pMapEditor; }

    // ToDo:
    inline DATABASE::DatabaseMgr& getDatabaseMgr() { return m_Project.getDatabaseMgr(); }

private:
    Project m_Project;
    MapEditorWidgetEditor *m_pMapEditor;
};
#endif