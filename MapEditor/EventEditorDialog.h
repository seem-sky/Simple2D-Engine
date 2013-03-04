#ifndef EVENT_EDITOR_WINDOW
#define EVENT_EDITOR_WINDOW

#include "UI/UI_EventEditorDialog.h"
#include <QtGui/QCloseEvent>
#include <EventScriptHolder.h>

class EventEditorDialog : public QDialog, Ui_EventEditorDialog
{
    Q_OBJECT
public:
    EventEditorDialog(QWidget *p_pParent = NULL);
    void SetEventScriptHolder(EVENT_SCRIPT::EventScriptHolder *p_pEventScriptHolder);

protected:
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.resizeEvent(this); }

private slots:
    void Accepted();

private:
    ModifyObject m_ResizeObj;
    EVENT_SCRIPT::EventScriptHolder *m_pObjectScriptHolder;
    EVENT_SCRIPT::EventScriptHolder m_EventScriptHolder;
};
#endif