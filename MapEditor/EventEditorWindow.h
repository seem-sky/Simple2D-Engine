#ifndef EVENT_EDITOR_WINDOW
#define EVENT_EDITOR_WINDOW

#include "UI/UI_EventEditorMainWindow.h"
#include <QtGui/QCloseEvent>

class EventEditorWindow : public QMainWindow, Ui_EventEditorWindow
{
public:
    EventEditorWindow(QMainWindow *p_pParent = NULL);
    ~EventEditorWindow(void);

protected:
    void closeEvent(QCloseEvent *p_Event);
    void resizeEvent(QResizeEvent *p_Event);
};
#endif