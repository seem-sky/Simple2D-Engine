#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "UI/UI_MainWindow.h"
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
    bool _loadDB(const QString& projectPath);
    bool _loadProject(const QString& sDir);
    void _setDBs();

public:
    MainWindow(QMainWindow* parent = 0);
    ~MainWindow(void);

private:
    Project m_Project;
    MapEditorWidgetEditor* m_pMapEditor;
};
#endif