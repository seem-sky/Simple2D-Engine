#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "UI/UI_MainWindow.h"
#include "ModifyObject.h"
#include "Point.h"
#include "MapDatabase.h"
#include "Config.h"

enum WindowAction
{
    WINDOW_DO,
    WINDOW_LOAD_DB,
    WINDOW_SAVE_DB,
};

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
private slots:
    void _openDatabase();
    void _newProject();
    void _loadProject();
    void _saveProject();
    void _closeProject();
    void _customUpdate();

signals:
    void WindowActionDone();
    void projectLoadDone();
    void projectSave();

private:
    bool _loadDB();
    bool _loadProject(const std::string &sDir);

protected:
    void resizeEvent(QResizeEvent *pEvent);
    void moveEvent(QMoveEvent *pEvent) { Config::Get()->setMainWindowPos(Point<uint32>(pEvent->pos().x(), pEvent->pos().y())); }

public:
    MainWindow(QMainWindow *parent = 0);
    ~MainWindow(void);

    inline void setNewTimer(QTimer *pTimer)
    {
        if (pTimer)
            m_pTimer = pTimer;
    }

    inline void setWindowAction(WindowAction action) { m_WindowAction = action; }
    inline MapEditor* getMapEditorWidget() const { return m_pMapEditor; }

private:
    ModifyObject m_ModifyObj;
    WindowAction m_WindowAction;
    QTimer *m_pTimer;
};
#endif