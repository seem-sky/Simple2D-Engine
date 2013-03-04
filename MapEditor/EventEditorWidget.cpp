#include "EventEditorWidget.h"
#include "moc_EventEditorWidget.h"
#include "StringAdditions.h"
#include "CommandWindow.h"
#include "ContextMenu.h"
#include "LocalVariableDialog.h"

using namespace EVENT_SCRIPT;
using namespace CONTEXT_MENU;

EventEditorWidget::EventEditorWidget(QWidget *p_pParent) : m_pEventScriptHolder(NULL), QWidget(p_pParent), Ui_EventEditor()
{
    setupUi(this);
    connect(m_pButtonNewPage, SIGNAL(clicked()), this, SLOT(ButtonEventPageAdded()));
    connect(m_pButtonDeletePage, SIGNAL(clicked()), this, SLOT(ButtonEventPageDeleted()));
    connect(m_pName, SIGNAL(editingFinished()), this, SLOT(PageNameChanged()));
    connect(m_pButtonEditLocalVariables, SIGNAL(clicked()), this, SLOT(LocalVariableDialogClicked()));
    connect(m_pEventEditorPages, SIGNAL(currentChanged(int)), this, SLOT(ScriptPageSelected(int)));
    m_ResizeObj.setWidget(m_pEventEditorPages, MODIFY_RESIZE);
    m_ResizeObj.setWidget(m_pPageConditions, MODIFY_RESIZE, QPoint(10+m_pButtonDeleteCondition->width(),0), MODIFY_DIRECTION_WIDTH);
    m_ResizeObj.setWidget(m_pButtonDeleteCondition, MODIFY_MOVE, QPoint(0, 0), MODIFY_DIRECTION_WIDTH);
    m_ResizeObj.setWidget(m_pButtonEditCondition, MODIFY_MOVE, QPoint(0, 0), MODIFY_DIRECTION_WIDTH);
    m_ResizeObj.setWidget(m_pButtonNewCondition, MODIFY_MOVE, QPoint(0, 0), MODIFY_DIRECTION_WIDTH);
}

void EventEditorWidget::LoadScriptsFromEventScriptHolder()
{
    if (!m_pEventScriptHolder)
        return;

    for (uint32 i = 0; i < m_pEventScriptHolder->GetEventScriptCount(); ++i)
    {
        if (EventScript *t_pScript = m_pEventScriptHolder->GetEventScript(i))
        {
            ScriptPage *t_pPage = new ScriptPage(t_pScript, m_pEventEditorPages);
            t_pPage->installEventFilter(this);
            m_pEventEditorPages->addTab(t_pPage, QString::fromStdString(t_pScript->GetName()));
            LoadCommandsFromEventScript(t_pScript, t_pPage);
        }
    }
}

void EventEditorWidget::LoadCommandsFromEventScript(const EventScript *p_pScript, ScriptPage *p_pPage)
{
    if (!p_pScript || !p_pPage)
        return;

    for (uint32 i = 0; i < p_pScript->GetCommandCount(); ++i)
    {
        EventScriptCommandPtr t_pCommand;
        if (p_pScript->GetCommand(i, t_pCommand) && p_pPage->AddNewCommandLine(t_pCommand, -1, false))
        {
            // only container could have children
            if (ScriptLineContainer *t_pScriptLine = (ScriptLineContainer*)p_pPage->topLevelItem(p_pPage->topLevelItemCount()-2))
            {
                if (t_pScriptLine->IsContainer())
                    LoadChildCommandsFromCommand(t_pScriptLine);
            }
        }
    }
}

void EventEditorWidget::LoadChildCommandsFromCommand(ScriptLineContainer *p_pScriptLine)
{
    if (!p_pScriptLine || !p_pScriptLine->GetCommand().get())
        return;

    if (EventScriptCommandContainer *t_pCommandContainer = (EventScriptCommandContainer*)p_pScriptLine->GetCommand().get())
    {
        for (uint32 i = 0; i < t_pCommandContainer->GetChildCommandCount(); ++i)
        {
            EventScriptCommandPtr t_pResult;
            if (t_pCommandContainer->GetChildCommand(i, t_pResult) && p_pScriptLine->AddNewCommandLine(t_pResult, -1, false))
            {
                // only container could have children
                if (ScriptLineContainer *t_pScriptLine = (ScriptLineContainer*)p_pScriptLine->child(p_pScriptLine->childCount()-2))
                {
                    if (t_pScriptLine->IsContainer())
                        LoadChildCommandsFromCommand(t_pScriptLine);
                }
            }
        }
    }
}

void EventEditorWidget::ScriptPageSelected(int p_Index)
{
    if (p_Index == -1)
        return;

    m_pName->setText(m_pEventEditorPages->tabText(p_Index));
    if (m_pEventScriptHolder)
    {
        if (EventScript *t_pScript = m_pEventScriptHolder->GetEventScript((uint32)p_Index))
        {
            if (VariableHolder *t_pVarHolder = t_pScript->GetLocalVariableHolder())
                UpdateLocalVariableCount(t_pVarHolder);
        }
    }
}

void EventEditorWidget::LocalVariableDialogClicked()
{
    if (ScriptPage *t_pPage = (ScriptPage*)m_pEventEditorPages->currentWidget())
    {
        VariableHolder *t_pHolder = t_pPage->GetVariableHolder();
        if (!t_pHolder)
            return;

        LocalVariableDialog t_Dialog(t_pHolder, this);
        if (t_Dialog.exec() == QDialog::Accepted)
            UpdateLocalVariableCount(t_pHolder);
    }
}

void EventEditorWidget::UpdateLocalVariableCount(const VariableHolder *p_pHolder)
{
    if (!p_pHolder)
        return;

    m_pBoolCount->setValue(p_pHolder->GetBoolCount());
    m_pIntegerCount->setValue(p_pHolder->GetIntegerCount());
    m_pFloatCount->setValue(p_pHolder->GetFloatCount());
    m_pStringCount->setValue(p_pHolder->GetStringCount());
}

void EventEditorWidget::ButtonEventPageAdded()
{
    if (!m_pEventScriptHolder)
        return;

    ScriptPage *t_pNewPage = new ScriptPage(m_pEventScriptHolder->InsertEventScript(), m_pEventEditorPages);
    t_pNewPage->installEventFilter(this);
    m_pEventEditorPages->addTab(t_pNewPage, "");
    m_pEventEditorPages->setCurrentWidget(t_pNewPage);
    QString t_sName("Page"+QString::number(m_pEventEditorPages->currentIndex()+1));
    m_pEventEditorPages->setTabText(m_pEventEditorPages->currentIndex(), t_sName);
    m_pName->setText(t_sName);
    PageNameChanged();
}

void EventEditorWidget::ButtonEventPageDeleted()
{
    if (QWidget *t_pWidget = m_pEventEditorPages->currentWidget())
    {
        ClearWidgets();
        m_pEventScriptHolder->DeleteEventScript(m_pEventEditorPages->currentIndex());
        m_pEventEditorPages->removeTab(m_pEventEditorPages->currentIndex());
        delete t_pWidget;
    }
}

void EventEditorWidget::PageNameChanged()
{
    if (m_pEventEditorPages->currentIndex() == -1)
        return;

    int32 t_Index = m_pEventEditorPages->currentIndex();
    if (t_Index < 0)
        return;

    m_pEventEditorPages->setTabText(t_Index, m_pName->text());
    if (m_pEventScriptHolder)
        m_pEventScriptHolder->SetScriptName(m_pName->text().toStdString(), (uint32)t_Index);
}

void EventEditorWidget::ClearWidgets()
{
    m_pName->clear();
    m_pBoolCount->clear();
    m_pIntegerCount->clear();
    m_pFloatCount->clear();
    m_pStringCount->clear();
    m_pPageConditions->clear();
}

/*#####
# ScriptContainer
#####*/
ScriptLine* ScriptContainer::GetNewScriptLine(const EventScriptCommandPtr &p_pCommand)
{
    if (p_pCommand.get()->GetCommandType() < COMMAND_CONTAINER)
        return new ScriptLine(p_pCommand);
    else
        return new ScriptLineContainer(p_pCommand);
}

/*#####
# ScriptPage
#####*/
ScriptPage::ScriptPage(EventScript *p_pScript, QWidget *p_pParent /* = NULL */) : m_pScript(p_pScript), QTreeWidget(p_pParent), ScriptContainer()
{
    AddNewCommandLine(EventScriptCommandPtr(new EventScriptCommand()), -1, false);
    viewport()->installEventFilter(this);
    setHeaderHidden(true);
    setIndentation(10);
}

bool ScriptPage::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    if (!p_pObj || !p_pEvent)
        return false;

    // open event command dialog
    if (p_pEvent->type() == QEvent::MouseButtonDblClick && ((QMouseEvent*)p_pEvent)->button() == Qt::LeftButton)
    {
        ScriptLine *t_pItem = (ScriptLine*)itemAt(((QMouseEvent*)p_pEvent)->pos());
        if (!t_pItem)
            return false;

        ScriptContainer *t_pContainer = NULL;
        if (t_pItem->parent())
            t_pContainer = (ScriptLineContainer*)t_pItem->parent();
        else
            t_pContainer = (ScriptPage*)t_pItem->treeWidget();
        if (t_pContainer)
        {
            CommandWindow t_Dialog(t_pContainer, this);
            t_Dialog.exec();
        }
    }
    // show menu
    else if (p_pEvent->type() == QEvent::MouseButtonRelease && ((QMouseEvent*)p_pEvent)->button() == Qt::RightButton)
    {
        if (QTreeWidgetItem *t_pItem = currentItem())
        {
            if (!((ScriptLine*)t_pItem)->GetCommand() || ((ScriptLine*)t_pItem)->GetCommand()->GetCommandType() == COMMAND_NONE)
                return false;

            ScriptLineContextMenu *t_pMenu = new ScriptLineContextMenu(this);
            t_pMenu->popup(mapToGlobal(((QMouseEvent*)p_pEvent)->pos()));
        }
    }
    else // pass the event on to the parent class
        return QWidget::eventFilter(p_pObj, p_pEvent);

    return false;
}

bool ScriptPage::AddNewCommandLine(const EventScriptCommandPtr &p_pCommand, int32 p_Row /*= -1*/, bool p_bInsertInHolder /*= true*/)
{
    if (!p_pCommand.get() || !m_pScript)
        return false;

    ScriptLine *t_pNewCommandLine = GetNewScriptLine(p_pCommand);
    if (!t_pNewCommandLine)
        return false;

    if (p_Row < 0)
        p_Row = topLevelItemCount() > 0 ? topLevelItemCount()-1 : 0;

    insertTopLevelItem(p_Row, t_pNewCommandLine);
    t_pNewCommandLine->setExpanded(true);
    if (p_bInsertInHolder)
        m_pScript->InsertCommand((uint32)p_Row, p_pCommand);
    return true;
}

void ScriptPage::RemoveCommandLine(ScriptLine *p_pCommand)
{
    if (!p_pCommand || !p_pCommand->GetCommand() || p_pCommand->GetCommand()->GetCommandType() == COMMAND_NONE)
        return;

    if (m_pScript)
        m_pScript->DeleteCommand(p_pCommand->GetCommand());
    if (QTreeWidgetItem *t_pItem = takeTopLevelItem(indexOfTopLevelItem(p_pCommand)))
        delete t_pItem;
}

/*#####
# ScriptLine
#####*/
ScriptLine::ScriptLine(const EventScriptCommandPtr &p_pCommand) : m_pCommand(p_pCommand), QTreeWidgetItem()
{
    // show comments with green color
    if (m_pCommand->GetCommandType() == COMMAND_COMMENT)
        setForeground(0, QBrush(QColor(70, 138, 30)));

    UpdateItemText();
}

void ScriptLine::UpdateItemText()
{
    if (!m_pCommand)
        return;

    setText(0, QString::fromStdString(m_pCommand->GetCommandText()));
}

/*#####
# ScriptLineContainer
#####*/
QTreeWidgetItem* ScriptLineContainer::GetCurrentChild() const
{
    for (int i = 0; i < childCount(); ++i)
    {
        if (QTreeWidgetItem *t_pItem = child(i))
        {
            if (t_pItem->isSelected())
                return t_pItem;
        }
    }

    return NULL;
}

int ScriptLineContainer::GetCurrentIndex() const
{
    if (QTreeWidgetItem *t_pItem = GetCurrentChild())
        return indexOfChild(t_pItem);

    return -1;
}

bool ScriptLineContainer::AddNewCommandLine(const EventScriptCommandPtr &p_pCommand, int32 p_Row /*= -1*/, bool p_bInsertInHolder /*= true*/)
{
    ScriptLine *t_pNewCommandLine = GetNewScriptLine(p_pCommand);
    if (!t_pNewCommandLine)
        return false;

    if (p_Row < 0)
        p_Row = childCount() > 0 ? childCount()-1 : 0;

    insertChild(p_Row, t_pNewCommandLine);
    if (p_bInsertInHolder && GetCommand())
        ((EventScriptCommandContainer*)GetCommand().get())->InsertChildCommand(p_Row, p_pCommand);
    t_pNewCommandLine->setExpanded(true);
    return true;
}

void ScriptLineContainer::RemoveCommandLine(ScriptLine *p_pCommand)
{
    if (!p_pCommand || !p_pCommand->GetCommand() || p_pCommand->GetCommand()->GetCommandType() == COMMAND_NONE)
        return;

    if (GetCommand() && GetCommand()->GetCommandType() >= COMMAND_CONTAINER)
        ((EventScriptCommandContainer*)GetCommand().get())->DeleteChildCommand(p_pCommand->GetCommand());
    removeChild(p_pCommand);
    delete p_pCommand;
}