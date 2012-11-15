#include "EventEditorWidget.h"
#include "moc_EventEditorWidget.h"
#include "StringAdditions.h"
#include "CommandWindow.h"
#include "ContextMenu.h"
#include "LocalVariableDialog.h"

using namespace EVENT_SCRIPT;
using namespace CONTEXT_MENU;

EventEditorWidget::EventEditorWidget(QWidget *p_pParent) : QWidget(p_pParent), Ui_EventEditor()
{
    setupUi(this);
    connect(m_pButtonNewPage, SIGNAL(clicked()), this, SLOT(ButtonEventPageAdded()));
    connect(m_pButtonDeletePage, SIGNAL(clicked()), this, SLOT(ButtonEventPageDeleted()));
    connect(m_pName, SIGNAL(editingFinished()), this, SLOT(PageNameChanged()));
    connect(m_pButtonEditLocalVariables, SIGNAL(clicked()), this, SLOT(LocalVariableDialogClicked()));
    connect(m_pEventEditorPages, SIGNAL(currentChanged(int)), this, SLOT(ScriptPageSelected(int)));
    m_ResizeObj.AddResizeWidget(m_pEventEditorPages);
    m_ResizeObj.AddResizeWidget(m_pPageConditions, QPoint(10+m_pButtonDeleteCondition->width(),0), MODIFY_RESIZE_WIDTH);
    m_ResizeObj.AddMoveWidget(m_pButtonDeleteCondition, QPoint(0, 0), MODIFY_MOVE_WIDTH);
    m_ResizeObj.AddMoveWidget(m_pButtonEditCondition, QPoint(0, 0), MODIFY_MOVE_WIDTH);
    m_ResizeObj.AddMoveWidget(m_pButtonNewCondition, QPoint(0, 0), MODIFY_MOVE_WIDTH);
}

void EventEditorWidget::ScriptPageSelected(int p_Index)
{
    if (p_Index == -1)
        return;

    m_pName->setText(m_pEventEditorPages->tabText(p_Index));
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
    ScriptPage *t_pNewPage = new ScriptPage(m_pEventEditorPages);
    t_pNewPage->installEventFilter(this);
    m_pEventEditorPages->addTab(t_pNewPage, "");
    m_pEventEditorPages->setCurrentWidget(t_pNewPage);
    QString t_sName("Page"+QString::number(m_pEventEditorPages->currentIndex()+1));
    m_pEventEditorPages->setTabText(m_pEventEditorPages->currentIndex(), t_sName);
    m_pName->setText(t_sName);
}

void EventEditorWidget::ButtonEventPageDeleted()
{
    if (QWidget *t_pWidget = m_pEventEditorPages->currentWidget())
    {
        ClearWidgets();
        m_pEventEditorPages->removeTab(m_pEventEditorPages->currentIndex());
        delete t_pWidget;
    }
}

void EventEditorWidget::PageNameChanged()
{
    if (m_pEventEditorPages->currentIndex() == -1)
        return;

    m_pEventEditorPages->setTabText(m_pEventEditorPages->currentIndex(), m_pName->text());
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
ScriptLine* ScriptContainer::GetNewScriptLine(EventScriptCommand *p_pCommand)
{
    if (!p_pCommand)
        return NULL;

    if (p_pCommand->GetCommandType() < COMMAND_CONTAINER)
        return new ScriptLine(p_pCommand);
    else
        return new ScriptLineContainer(p_pCommand);
}

/*#####
# ScriptPage
#####*/
ScriptPage::ScriptPage(QWidget *p_pParent /* = NULL */) : QTreeWidget(p_pParent)
{
    AddNewCommandLine(new EventScriptCommand());
    viewport()->installEventFilter(this);
    setHeaderHidden(true);
    setIndentation(0);
}

bool ScriptPage::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    if (!p_pObj || !p_pEvent)
        return false;

    // open event command dialog
    if (p_pEvent->type() == QEvent::MouseButtonDblClick && ((QMouseEvent*)p_pEvent)->button() == Qt::LeftButton)
    {
        QTreeWidgetItem *t_pItem = itemAt(((QMouseEvent*)p_pEvent)->pos());
        if (!t_pItem)
            setCurrentItem (t_pItem, QItemSelectionModel::Clear);

        CommandWindow t_Dialog(this);
        t_Dialog.exec();
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

bool ScriptPage::AddNewCommandLine(EVENT_SCRIPT::EventScriptCommand *p_pCommand, int p_Row /*= -1*/)
{
    if (!p_pCommand)
        return false;

    ScriptLine *t_pNewCommandLine = GetNewScriptLine(p_pCommand);
    if (!t_pNewCommandLine)
        return false;

    if (p_Row < 0)
        insertTopLevelItem(topLevelItemCount() > 0 ? topLevelItemCount()-1 : 0, t_pNewCommandLine);
    else
        insertTopLevelItem(p_Row, t_pNewCommandLine);
    
    t_pNewCommandLine->setExpanded(true);
    return true;
}

void ScriptPage::RemoveCommandLine(ScriptLine *p_pCommand)
{
    if (!p_pCommand || !p_pCommand->GetCommand() || p_pCommand->GetCommand()->GetCommandType() == COMMAND_NONE)
        return;

    if (QTreeWidgetItem *t_pItem = takeTopLevelItem(indexOfTopLevelItem(p_pCommand)))
        delete t_pItem;
}

/*#####
# ScriptLine
#####*/
ScriptLine::ScriptLine(EventScriptCommand *p_pCommand) : m_pCommand(p_pCommand), QTreeWidgetItem()
{
    if (!m_pCommand)
        return;

    // show comments with green color
    if (m_pCommand->GetCommandType() == COMMAND_COMMENT)
        setForeground(0, QBrush(QColor(70, 138, 30)));

    UpdateItemText();
}

ScriptLine::~ScriptLine()
{
    if (m_pCommand)
    {
        delete m_pCommand;
        m_pCommand = NULL;
    }
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
bool ScriptLineContainer::AddNewCommandLine(EventScriptCommand *p_pCommand, int p_Row /*= -1*/)
{
    if (!p_pCommand)
        return false;

    ScriptLine *t_pNewCommandLine = GetNewScriptLine(p_pCommand);
    if (!t_pNewCommandLine)
        return false;

    if (p_Row < 0)
        insertChild(childCount() > 0 ? childCount()-1 : 0, t_pNewCommandLine);
    else
        insertChild(p_Row, t_pNewCommandLine);

    if (GetCommand())
        ((EventScriptCommandContainer*)GetCommand())->InsertChildCommand(p_Row, p_pCommand);
    t_pNewCommandLine->setExpanded(true);
    return true;
}

void ScriptLineContainer::RemoveCommandLine(ScriptLine *p_pCommand)
{
    if (!p_pCommand || !p_pCommand->GetCommand() || p_pCommand->GetCommand()->GetCommandType() == COMMAND_NONE)
        return;

    removeChild(p_pCommand);
    delete p_pCommand;
}