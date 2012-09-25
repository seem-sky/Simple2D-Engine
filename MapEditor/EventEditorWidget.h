#ifndef EVENT_EDITOR_WIDGET
#define EVENT_EDITOR_WIDGET

#include "UI/UI_EventEditor.h"
#include <QtGui/QListWidget>
#include "ScriptDatabase.h"
#include "ResizeWidget.h"

class ScriptPage : public QListWidget
{
public:
    ScriptPage(QWidget *p_pParent = NULL);

    void AddNewCommandLine(EVENT_SCRIPT::EventScriptCommand *p_pCommand);

protected:
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);
};

class ScriptLine : public QListWidgetItem
{
public:
    ScriptLine(EVENT_SCRIPT::EventScriptCommand *p_pCommandParent, QListWidget *p_pWidgetParent) : m_pCommand(p_pCommandParent), QListWidgetItem(p_pWidgetParent)
    {
        if (m_pCommand)
            setText(QString(m_pCommand->GetCommandText().c_str()));
    }

private:
    EVENT_SCRIPT::EventScriptCommand *m_pCommand;
};

class EventEditorWidget : public QWidget, Ui_EventEditor
{
    Q_OBJECT
public:
    EventEditorWidget(QWidget *p_pParent = NULL);
    ~EventEditorWidget(void);

protected:
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.ResizeEvent(this); }

private slots:
    void ButtonEventPageAdded();
    void ButtonEventPageDeleted();
    void PageNameChanged();

private:
    ResizeWidget m_ResizeObj;
};
#endif