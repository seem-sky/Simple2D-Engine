#ifndef EVENT_EDITOR_WIDGET
#define EVENT_EDITOR_WIDGET

#include "UI/UI_EventEditor.h"
#include <QtGui/QTreeWidget>
#include "ModifyObject.h"
#include <EventScriptHolder.h>

class ScriptLine : public QTreeWidgetItem
{
public:
    ScriptLine(const EVENT_SCRIPT::EventScriptCommandPtr &p_pCommand);

    inline EVENT_SCRIPT::EventScriptCommandPtr GetCommand() { return m_pCommand; }

    void UpdateItemText();
    virtual bool IsContainer() const { return false; }

private:
    EVENT_SCRIPT::EventScriptCommandPtr m_pCommand;
};

class ScriptContainer
{
public:
    virtual bool AddNewCommandLine(const EVENT_SCRIPT::EventScriptCommandPtr &p_pCommand, int32 p_Row = -1, bool p_bInsertInHolder = true) = 0;
    virtual void RemoveCommandLine(ScriptLine *p_pCommand) = 0;

    virtual bool IsCommand() const = 0;

    ScriptLine* GetNewScriptLine(const EVENT_SCRIPT::EventScriptCommandPtr &p_pCommand);
    virtual int32 GetCurrentIndex() const = 0;
};

class ScriptPage : public ScriptContainer, public QTreeWidget
{
public:
    ScriptPage(EVENT_SCRIPT::EventScript *p_pScript, QWidget *p_pParent = NULL);

    bool AddNewCommandLine(const EVENT_SCRIPT::EventScriptCommandPtr &p_pCommand, int32 p_Row = -1, bool p_bInsertInHolder = true);
    void RemoveCommandLine(ScriptLine *p_pCommand);
    bool IsCommand() const { return false; }
    inline VariableHolder* GetVariableHolder()
    {
        if (!m_pScript)
            return NULL;
        return m_pScript->GetLocalVariableHolder();
    }

    int32 GetCurrentIndex() const { return indexOfTopLevelItem(currentItem()); }

protected:
    bool eventFilter(QObject *p_pObj, QEvent *p_pEvent);
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.resizeEvent(this); }

    ModifyObject m_ResizeObj;
    EVENT_SCRIPT::EventScript *m_pScript;
};

class ScriptLineContainer : public ScriptLine, public ScriptContainer
{
public:
    ScriptLineContainer(const EVENT_SCRIPT::EventScriptCommandPtr p_pCommand)  : ScriptLine(p_pCommand), ScriptContainer()
    {
        AddNewCommandLine(EVENT_SCRIPT::EventScriptCommandPtr(new EVENT_SCRIPT::EventScriptCommand()), -1, false);
    }

    bool AddNewCommandLine(const EVENT_SCRIPT::EventScriptCommandPtr &p_pCommand, int32 p_Row = -1, bool p_bInsertInHolder = true);
    void RemoveCommandLine(ScriptLine *p_pCommand);
    bool IsCommand() const { return true; }
    bool IsContainer() const { return true; }

    int32 GetCurrentIndex() const;
    QTreeWidgetItem* GetCurrentChild() const;
};

class EventEditorWidget : public QWidget, Ui_EventEditor
{
    Q_OBJECT
public:
    EventEditorWidget(QWidget *p_pParent = NULL);
    inline void SetEventScriptHolder(EVENT_SCRIPT::EventScriptHolder *p_pEventScriptHolder)
    {
        if (p_pEventScriptHolder)
        {
            m_pEventScriptHolder = p_pEventScriptHolder;
            LoadScriptsFromEventScriptHolder();
        }
    }

protected:
    void resizeEvent(QResizeEvent *p_pEvent) { m_ResizeObj.resizeEvent(this); }

private slots:
    void ScriptPageSelected(int p_Index);
    void ButtonEventPageAdded();
    void ButtonEventPageDeleted();
    void PageNameChanged();
    void LocalVariableDialogClicked();

private:
    void LoadScriptsFromEventScriptHolder();
    void LoadCommandsFromEventScript(const EVENT_SCRIPT::EventScript *p_pScript, ScriptPage *p_pPage);
    void LoadChildCommandsFromCommand(ScriptLineContainer *p_pScriptLine);
    void UpdateLocalVariableCount(const VariableHolder *p_pHolder);
    void ClearWidgets();

    ModifyObject m_ResizeObj;
    EVENT_SCRIPT::EventScriptHolder *m_pEventScriptHolder;
};
#endif