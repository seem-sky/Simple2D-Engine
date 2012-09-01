#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#define EDITOR

#include "UI/UI_MainWindow.h"

enum WINDOW_ACTION
{
    WINDOW_DO,
    WINDOW_LOAD_DB,
    WINDOW_SAVE_DB,
};

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QMainWindow *parent = 0);
    ~MainWindow(void);

    inline void SetNewTimer(QTimer *p_pTimer)
    {
        if (p_pTimer)
            m_pTimer = p_pTimer;
    }

    inline void SetWindowAction(WINDOW_ACTION p_Action) { m_WindowAction = p_Action; }

private:
    bool LoadDB();
    WINDOW_ACTION m_WindowAction;
    QTimer *m_pTimer;

private slots:
    void OpenDatabase();
    void NewProject();
    void LoadProject();
    void CustomUpdate();

signals:
    void WindowActionDone();
};
#endif