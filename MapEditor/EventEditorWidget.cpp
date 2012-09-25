#include "EventEditorWidget.h"
#include "moc_EventEditorWidget.h"
#include "StringAdditions.h"
#include "CommandWindow.h"

using namespace EVENT_SCRIPT;

EventEditorWidget::EventEditorWidget(QWidget *p_pParent) : QWidget(p_pParent), Ui_EventEditor()
{
    setupUi(this);
    connect(m_pButtonNewPage, SIGNAL(clicked()), this, SLOT(ButtonEventPageAdded()));
    connect(m_pButtonDeletePage, SIGNAL(clicked()), this, SLOT(ButtonEventPageDeleted()));
    connect(m_pName, SIGNAL(clicked()), this, SLOT(PageNameChanged()));
    m_ResizeObj.AddResizeWidget(m_pEventEditorPages);
}

EventEditorWidget::~EventEditorWidget(void)
{
}

void EventEditorWidget::ButtonEventPageAdded()
{
    ScriptPage *t_pNewPage = new ScriptPage(m_pEventEditorPages);
    t_pNewPage->installEventFilter(this);
    m_pEventEditorPages->addTab(t_pNewPage, "");
    m_pEventEditorPages->setCurrentWidget(t_pNewPage);
    QString t_sName(("Page"+ToString(m_pEventEditorPages->currentIndex()+1)).c_str());
    m_pEventEditorPages->setTabText(m_pEventEditorPages->currentIndex(), t_sName);
    m_pName->setText(t_sName);
}

void EventEditorWidget::ButtonEventPageDeleted()
{

}

void EventEditorWidget::PageNameChanged()
{
    m_pEventEditorPages->setTabText(m_pEventEditorPages->currentIndex(), m_pName->text());
}

/*#####
# ScriptList
#####*/
ScriptPage::ScriptPage(QWidget *p_pParent /* = NULL */) : QListWidget(p_pParent)
{
    viewport()->installEventFilter(this);
}

bool ScriptPage::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    if (p_pEvent->type() == QEvent::MouseButtonDblClick)
    {
        CommandWindow t_Dialog(this);
        t_Dialog.exec();
    }
    else // pass the event on to the parent class
        return QWidget::eventFilter(p_pObj, p_pEvent);

    return false;
}

void ScriptPage::AddNewCommandLine(EventScriptCommand *p_pCommand)
{
    addItem(new ScriptLine(p_pCommand, this));
}