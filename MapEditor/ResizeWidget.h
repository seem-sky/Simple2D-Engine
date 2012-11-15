#ifndef RESIZE_WIDGET_H
#define RESIZE_WIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QResizeEvent>
#include <map>
#include <Global.h>

enum ModifyFlags
{
    MODIFY_NONE             = 0x000,
    MODIFY_RESIZE_WIDTH     = 0x001,
    MODIFY_RESIZE_HEIGHT    = 0x002,
    MODIFY_MOVE_WIDTH       = 0x001,
    MODIFY_MOVE_HEIGHT      = 0x002,
};

struct Modifyer
{
    Modifyer(QPoint p_Border, uint32 p_ModifyFlag) : m_Border(p_Border), m_ModifyFlags(p_ModifyFlag) {}

    inline bool HasModifyFlag(ModifyFlags p_Flag) const { return (m_ModifyFlags & p_Flag) != 0; }
    inline void AddModifyFlag(ModifyFlags p_Flag) { m_ModifyFlags |= p_Flag; }
    inline void RemoveModifyFlag(ModifyFlags p_Flag)
    {
        if (HasModifyFlag(p_Flag))
            m_ModifyFlags ^= p_Flag;
    }

    uint32 m_ModifyFlags;
    QPoint m_Border;
};

typedef std::map<QWidget*, Modifyer> ResizeObjectList;

class ResizeWidget
{
public:
    void AddResizeWidget(QWidget *p_pWidget, QPoint p_Border = QPoint(0,0), uint32 p_ModifyerFlags = MODIFY_RESIZE_WIDTH | MODIFY_RESIZE_HEIGHT);
    void RemoveResizeWidget(QWidget *p_pWidget);

    void AddMoveWidget(QWidget *p_pWidget, QPoint p_Border = QPoint(0,0), uint32 p_ModifyerFlags = MODIFY_MOVE_WIDTH | MODIFY_MOVE_HEIGHT);
    void RemoveMoveWidget(QWidget *p_pWidget);

    void ResizeEvent(QWidget *p_pResizedWidget);

private:
    ResizeObjectList m_plResizeWidgets;
    ResizeObjectList m_plMoveWidgets;
};
#endif