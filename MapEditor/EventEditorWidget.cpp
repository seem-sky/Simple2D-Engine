#include "EventEditorWidget.h"
#include "moc_EventEditorWidget.h"
#include "StringAdditions.h"

EventEditorWidget::EventEditorWidget(QWidget *p_pParent) : QWidget(p_pParent), Ui_EventEditor()
{
    setupUi(this);
    connect(ButtonNewPage, SIGNAL(clicked()), this, SLOT(ButtonAddPage()));
}

EventEditorWidget::~EventEditorWidget(void)
{
}

void EventEditorWidget::resizeEvent(QResizeEvent *p_Event)
{
    QRect t_TabSize = EventEditorPages->geometry();
    QSize t_WidgetSize = size();
    EventEditorPages->resize(t_WidgetSize.width() - t_TabSize.left(), t_WidgetSize.height() - t_TabSize.top());
}

void EventEditorWidget::ButtonAddPage()
{
    ScriptPage *t_pNewPage = new ScriptPage(EventEditorPages);
    EventEditorPages->addTab(t_pNewPage, "");
    EventEditorPages->setCurrentWidget(t_pNewPage);
    EventEditorPages->setTabText(EventEditorPages->currentIndex(), QString(("Page"+ToString(EventEditorPages->currentIndex()+1)).c_str()));
}


/*#####
# ScriptList
#####*/
ScriptPage::ScriptPage(QWidget *p_pParent /* = NULL */) : QWidget(p_pParent), m_pEventList(new QListWidget(this))
{
}

void ScriptPage::resizeEvent(QResizeEvent *p_Event)
{
    m_pEventList->resize(size());
}