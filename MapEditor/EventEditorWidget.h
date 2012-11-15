#ifndef EVENT_EDITOR_WIDGET
#define EVENT_EDITOR_WIDGET

#include "UI/UI_EventEditor.h"
#include <QtGui/QTreeWidget>
#include "ScriptDatabase.h"
#include "ResizeWidget.h"
#include <QtGui/QStyledItemDelegate>
#include <VariableHolder.h>

class ScriptLine : public QTreeWidgetItem
{
public:
    ScriptLine(EVENT_SCRIPT::EventScriptCommand *p_pCommand);
    ~ScriptLine();

    virtual EVENT_SCRIPT::EventScriptCommand* GetCommand() { return m_pCommand; }

    virtual void UpdateItemText();

    virtual bool IsContainer() const { return false; }

private:
    EVENT_SCRIPT::EventScriptCommand *m_pCommand;
};

class ScriptContainer
{
public:
    virtual bool AddNewCommandLine(EVENT_SCRIPT::EventScriptCommand *p_pCommand, int p_Row = -1) = 0;
    virtual void RemoveCommandLine(ScriptLine *p_pCommand) = 0;

    virtual bool IsCommand() const = 0;

    ScriptLine* GetNewScriptLine(EVENT_SCRIPT::EventScriptCommand *p_pCommand);
};

class ScriptPage : public ScriptContainer, public QTreeWidget
{
public:
    ScriptPage(QWidget *p_pParent = NULL);

    bool AddNewCommandLine(EVENT_SCRIPT::EventScriptCommand *p_pCommand, int p_Row = -1);
    void RemoveCommandLine(ScriptLine *p_pCommand);
    bool IsCommand() const { return false; }
    inline VariableHolder* GetVariableHolder() { return &m_pLocalVariables; }

protected:
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.ResizeEvent(this); }

    ResizeWidget m_ResizeObj;
    VariableHolder m_pLocalVariables;
};

class ScriptLineContainer : public ScriptLine, public ScriptContainer
{
public:
    ScriptLineContainer(EVENT_SCRIPT::EventScriptCommand *p_pCommand)  : ScriptLine(p_pCommand), ScriptContainer()
    {
        AddNewCommandLine(new EVENT_SCRIPT::EventScriptCommand());
    }

    bool AddNewCommandLine(EVENT_SCRIPT::EventScriptCommand *p_pCommand, int p_Row = -1);
    void RemoveCommandLine(ScriptLine *p_pCommand);
    bool IsCommand() const { return true; }
    bool IsContainer() const { return true; }
};

class EventEditorWidget : public QWidget, Ui_EventEditor
{
    Q_OBJECT
public:
    EventEditorWidget(QWidget *p_pParent = NULL);

protected:
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.ResizeEvent(this); }

private slots:
    void ScriptPageSelected(int p_Index);
    void ButtonEventPageAdded();
    void ButtonEventPageDeleted();
    void PageNameChanged();
    void LocalVariableDialogClicked();

private:
    void UpdateLocalVariableCount(const VariableHolder *p_pHolder);
    void ClearWidgets();

    ResizeWidget m_ResizeObj;
};
#endif