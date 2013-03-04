#include "EventEditorDialog.h"
#include "moc_EventEditorDialog.h"

EventEditorDialog::EventEditorDialog(QWidget *p_pParent) : QDialog(p_pParent), Ui_EventEditorDialog()
{
    setupUi(this);

    m_ResizeObj.setWidget(m_pEventEditor, MODIFY_RESIZE, QPoint(10, 10+m_pButtons->height()));
    m_ResizeObj.setWidget(m_pButtons, MODIFY_MOVE, QPoint(10, 10));
    connect(m_pButtons, SIGNAL(accepted()), this, SLOT(Accepted()));
}

void EventEditorDialog::SetEventScriptHolder(EVENT_SCRIPT::EventScriptHolder *p_pEventScriptHolder)
{
    if (m_pEventEditor && p_pEventScriptHolder)
    {
        m_pObjectScriptHolder = p_pEventScriptHolder;
        m_EventScriptHolder = *m_pObjectScriptHolder;
        m_pEventEditor->SetEventScriptHolder(&m_EventScriptHolder);
    }
}

void EventEditorDialog::Accepted()
{
    if (!m_pObjectScriptHolder)
        return;

    *m_pObjectScriptHolder = m_EventScriptHolder;
    //m_pObjectScriptHolder->DeleteCommandsOfTypeInAllScripts(EVENT_SCRIPT::COMMAND_CONTAINER);
    //m_pObjectScriptHolder->DeleteCommandsOfTypeInAllScripts(EVENT_SCRIPT::COMMAND_NONE);
    accept();
}