#include "EventEditorWindow.h"

EventEditorWindow::EventEditorWindow(QMainWindow *p_pParent) : QMainWindow(p_pParent), Ui_EventEditorWindow()
{
    setupUi(this);

    // set minimum window size
    QRect t_TabGeo = EventEditor->geometry();
    const int BORDER = t_TabGeo.top();
    QSize t_ButtonSize = ButtonApply->size();
    QSize t_EEminSize = EventEditor->minimumSize();
    QSize t_MinWindowSize;
    t_MinWindowSize.setWidth(t_TabGeo.left() + t_TabGeo.right() + BORDER);
    t_MinWindowSize.setHeight(t_TabGeo.top() + t_TabGeo.bottom() + BORDER + t_ButtonSize.height());
    setMinimumSize(t_MinWindowSize);
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
void EventEditorWindow::resizeEvent(QResizeEvent *p_Event)
{
    QRect t_TabGeo = EventEditor->geometry();
    const int BORDER = t_TabGeo.top();
    QSize t_WidgetSize = size();
    QSize t_ButtonSize = ButtonApply->size();
    
    // resize editor widget
    EventEditor->resize(t_WidgetSize.width() - t_TabGeo.left() - BORDER, t_WidgetSize.height() - t_TabGeo.top() - BORDER - t_ButtonSize.height());

    // repos buttons
    ButtonApply->move(t_WidgetSize.width() - ButtonApply->width() - BORDER, t_WidgetSize.height() - ButtonApply->height() - BORDER);
    ButtonCancel->move(t_WidgetSize.width() - ButtonCancel->width()*2 - BORDER, t_WidgetSize.height() - ButtonCancel->height() - BORDER);
    ButtonOK->move(t_WidgetSize.width() - ButtonOK->width()*3 - BORDER, t_WidgetSize.height() - ButtonOK->height() - BORDER);
}
