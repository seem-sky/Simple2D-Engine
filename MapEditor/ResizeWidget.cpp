#include "ResizeWidget.h"

void ResizeWidget::ResizeEvent(QWidget *p_pResizedWidget)
{
    if (!p_pResizedWidget)
        return;

    QSize t_ParentSize = p_pResizedWidget->size();
    // resize widgets
    for (ResizeObjectList::iterator t_Itr = m_plResizeWidgets.begin(); t_Itr != m_plResizeWidgets.end(); ++t_Itr)
    {
        if (!t_Itr->first)
            continue;

        QRect t_TabSize = t_Itr->first->geometry();
        t_Itr->first->resize(t_ParentSize.width() - t_TabSize.left() - t_Itr->second.x(), t_ParentSize.height() - t_TabSize.top() - t_Itr->second.y());
    }

    // move objects
    for (ResizeObjectList::iterator t_Itr = m_plMoveWidgets.begin(); t_Itr != m_plMoveWidgets.end(); ++t_Itr)
    {
        if (!t_Itr->first)
            continue;

        t_Itr->first->move(t_ParentSize.width() - t_Itr->first->width() - t_Itr->second.x(), t_ParentSize.height() - t_Itr->first->height() - t_Itr->second.y());
    }
}

void ResizeWidget::RemoveResizeWidget(QWidget *p_pWidget)
{
    if (!p_pWidget)
        return;

    ResizeObjectList::iterator t_Itr = m_plResizeWidgets.find(p_pWidget);
    if (t_Itr != m_plResizeWidgets.end())
        m_plResizeWidgets.erase(t_Itr);
}