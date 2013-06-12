#include "ModifyObject.h"

ModifyObject::Modifyer::Modifyer(const QPoint &border, uint32 uiDirectionFlag) : m_Border(border), m_uiDirectionFlag(uiDirectionFlag)
{}

bool ModifyObject::_findWidget(QWidget* pWidget, ModifyState state, ModifyWidgetMap::iterator& resultItr)
{
    if (!pWidget)
        return false;

    ModifyWidgetMap::iterator itr = (state == MODIFY_MOVE ? m_pMoveWidgetMap.find(pWidget) : m_pModifyWidgetMap.find(pWidget));
    if (itr != (state == MODIFY_MOVE ? m_pMoveWidgetMap.end() : m_pModifyWidgetMap.end()))
    {
        resultItr = itr;
        return true;
    }

    return false;
}

void ModifyObject::resizeEvent(QWidget *pResizedWidget)
{
    if (!pResizedWidget)
        return;

    QSize parentSize = pResizedWidget->size();
    // resize widgets
    for (ModifyWidgetMap::iterator itr = m_pModifyWidgetMap.begin(); itr != m_pModifyWidgetMap.end(); ++itr)
    {
        if (!itr->first)
            continue;

        QPoint widgetPosition = itr->first->pos();
        QSize widgetSize = itr->first->size();
        itr->first->resize(itr->second.HasDirectionFlag(MODIFY_DIRECTION_WIDTH) ? parentSize.width() - widgetPosition.x() - itr->second.m_Border.x() : itr->first->width(),
            itr->second.HasDirectionFlag(MODIFY_DIRECTION_HEIGHT) ? parentSize.height() - widgetPosition.y() - itr->second.m_Border.y() : itr->first->height());
    }

    // move widgets
    for (ModifyWidgetMap::iterator itr = m_pMoveWidgetMap.begin(); itr != m_pMoveWidgetMap.end(); ++itr)
    {
        if (!itr->first)
            continue;

        QPoint widgetPosition = itr->first->pos();
        QSize widgetSize = itr->first->size();
        itr->first->move(itr->second.HasDirectionFlag(MODIFY_DIRECTION_WIDTH) ? parentSize.width() - widgetSize.width() - itr->second.m_Border.x() : itr->first->x(),
            itr->second.HasDirectionFlag(MODIFY_DIRECTION_HEIGHT) ? parentSize.height() - widgetSize.height() - itr->second.m_Border.y() : itr->first->y());
    }
}

void ModifyObject::removeWidget(QWidget *pWidget)
{
    removeWidget(pWidget, MODIFY_MOVE);
    removeWidget(pWidget, MODIFY_RESIZE);
}

void ModifyObject::removeWidget(QWidget *pWidget, ModifyState state)
{
    ModifyWidgetMap::iterator itr;
    if (_findWidget(pWidget, state, itr))
    {
        if (state == MODIFY_MOVE)
            m_pMoveWidgetMap.erase(itr);
        else
            m_pModifyWidgetMap.erase(itr);
    }
}

void ModifyObject::setWidget(QWidget *pWidget, ModifyState state, QPoint border, uint32 directionFlag)
{
    if (!pWidget)
        return;

    ModifyWidgetMap::iterator itr;
    if (_findWidget(pWidget, state, itr))
        itr->second = Modifyer(border, directionFlag);
    else
    {
        if (state == MODIFY_MOVE)
            m_pMoveWidgetMap.insert(std::make_pair(pWidget, Modifyer(border, directionFlag)));
        else
            m_pModifyWidgetMap.insert(std::make_pair(pWidget, Modifyer(border, directionFlag)));
    }
}