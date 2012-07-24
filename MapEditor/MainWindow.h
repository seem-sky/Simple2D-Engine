#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#define EDITOR

#include "UI/UI_MainWindow.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QMainWindow *parent = 0);
    ~MainWindow(void);

    QString GetProjectDir() { return m_sProjectDir; }

protected:
    enum WINDOW_ACTION
    {
        WINDOW_DO,
        WINDOW_LOAD_DB,
    };

private:
    WINDOW_ACTION m_WindowAction;
    QTimer *m_pTimer;

    QString m_sProjectDir;

private slots:
    void OpenDatabase();
    void NewProject();
    void LoadProject();
    void CustomUpdate();
};
#endif