#ifndef RESIZE_WIDGET_H
#define RESIZE_WIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QResizeEvent>
#include <map>
#include "Global.h"

enum ModifyDirectionFlag
{
    MODIFY_DIRECTION_WIDTH  = 0x01,
    MODIFY_DIRECTION_HEIGHT = 0x02
};

enum ModifyState
{
    MODIFY_MOVE,
    MODIFY_RESIZE
};

class ModifyObject
{
private:
    struct Modifyer
    {
        Modifyer(const QPoint &border, uint32 uiDirectionFlag);
        inline bool HasDirectionFlag(ModifyDirectionFlag uiFlag) const { return (m_uiDirectionFlag & uiFlag) != 0; }
        inline void AddDirectionFlag(ModifyDirectionFlag uiFlag)
        {
            if (!HasDirectionFlag(uiFlag))
                m_uiDirectionFlag |= uiFlag;
        }

        inline void RemoveModifyFlag(ModifyDirectionFlag uiFlag)
        {
            if (HasDirectionFlag(uiFlag))
                m_uiDirectionFlag ^= uiFlag;
        }

        uint32 m_uiDirectionFlag;
        QPoint m_Border;
    };
    typedef std::map<QWidget*, Modifyer> ModifyWidgetMap;

    bool _findWidget(QWidget* pWidget, ModifyState state, ModifyWidgetMap::iterator& resultItr);

public:
    void setWidget(QWidget *pWidget, ModifyState state, QPoint border = QPoint(0, 0), uint32 directionFlag = MODIFY_DIRECTION_WIDTH | MODIFY_DIRECTION_HEIGHT);
    void removeWidget(QWidget *pWidget, ModifyState state);
    void removeWidget(QWidget *pWidget);

    void resizeEvent(QWidget *pResizedWidget);

private:
    ModifyWidgetMap m_pModifyWidgetMap;
    ModifyWidgetMap m_pMoveWidgetMap;
};
#endif