#ifndef EVENT_EDITOR_WINDOW
#define EVENT_EDITOR_WINDOW

#include "UI/UI_EventEditorMainWindow.h"
#include <QtGui/QCloseEvent>

class EventEditorWindow : public QMainWindow, Ui_EventEditorWindow
{
public:
    EventEditorWindow(QWidget *p_pParent = NULL);
    ~EventEditorWindow(void);

protected:
    void closeEvent(QCloseEvent *p_Event);
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.ResizeEvent(this); }

private:
    ResizeWidget m_ResizeObj;
};
#endif