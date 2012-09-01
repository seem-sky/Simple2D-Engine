#ifndef RESIZE_WIDGET_H
#define RESIZE_WIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QResizeEvent>
#include <map>

typedef std::map<QWidget*, QPoint> ResizeObjectList;

class ResizeWidget
{
public:
    void AddResizeWidget(QWidget *p_pWidget, QPoint p_Border = QPoint(0,0)) { if (p_pWidget) m_plResizeWidgets.insert(std::make_pair(p_pWidget, p_Border)); }
    void RemoveResizeWidget(QWidget *p_pWidget);

    void AddMoveWidget(QWidget *p_pWidget, QPoint p_Border = QPoint(0,0))  { if (p_pWidget) m_plMoveWidgets.insert(std::make_pair(p_pWidget, p_Border)); }
    void RemoveMoveWidget(QWidget *p_pWidget);

    void ResizeEvent(QWidget *p_pResizedWidget);

private:
    ResizeObjectList m_plResizeWidgets;
    ResizeObjectList m_plMoveWidgets;
};
#endif