#ifndef EVENT_EDITOR_WIDGET
#define EVENT_EDITOR_WIDGET

#include "UI/UI_EventEditor.h"
#include <QtGui/QListWidget>
#include "ScriptDatabase.h"

class ScriptPage : public QWidget
{
public:
    ScriptPage(QWidget *p_pParent = NULL);

protected:
    void resizeEvent(QResizeEvent *p_Event);

private:
    QListWidget *m_pEventList;
};

class ScriptLine : public QListWidgetItem
{
public:
    ScriptLine(QListWidget *p_pParent);

private:
    EVENT_SCRIPT::EventCommand m_Command;
};

class EventEditorWidget : public QWidget, Ui_EventEditor
{
    Q_OBJECT
public:
    EventEditorWidget(QWidget *p_pParent = NULL);
    ~EventEditorWidget(void);

protected:
    void resizeEvent(QResizeEvent *p_Event);

private slots:
    void ButtonAddPage();
};
#endif