#include "EventEditorWindow.h"

EventEditorWindow::EventEditorWindow(QWidget *p_pParent) : QMainWindow(p_pParent), Ui_EventEditorWindow()
{
    setupUi(this);

    m_ResizeObj.AddResizeWidget(m_pEventEditor, QPoint(10, 10+m_pButtonApply->height()));
    m_ResizeObj.AddMoveWidget(m_pButtonApply, QPoint(10, 10));
    m_ResizeObj.AddMoveWidget(m_pButtonCancel, QPoint(m_pButtonApply->width()+10, 10));
    m_ResizeObj.AddMoveWidget(m_pButtonOK, QPoint(m_pButtonApply->width()+m_pButtonCancel->width()+10, 10));
}

EventEditorWindow::~EventEditorWindow(void)
{
    if (QWidget *t_pParent = (QWidget*)parent())
        t_pParent->setEnabled(true);
}

void EventEditorWindow::closeEvent(QCloseEvent *p_Event)
{
    p_Event->accept();

    delete this;
}