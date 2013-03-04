#include "ModifyObject.h"

ModifyObject::Modifyer::Modifyer(const QPoint &border, ModifyState state, uint32 uiDirectionFlag) : m_Border(border), m_ModifyState(state),
m_uiDirectionFlag(uiDirectionFlag)
{}

bool ModifyObject::_findWidget(QWidget* pWidget, ModifyWidgetMap::iterator& resultItr)
{
    if (!pWidget)
        return false;

    ModifyWidgetMap::iterator itr = m_pModifyWidgetMap.find(pWidget);
    if (itr != m_pModifyWidgetMap.end())
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
        switch (itr->second.m_ModifyState)
        {
        case MODIFY_MOVE:
            itr->first->move(itr->second.HasDirectionFlag(MODIFY_DIRECTION_WIDTH) ? parentSize.width() - widgetSize.width() - itr->second.m_Border.x() : itr->first->x(),
                itr->second.HasDirectionFlag(MODIFY_DIRECTION_HEIGHT) ? parentSize.height() - widgetSize.height() - itr->second.m_Border.y() : itr->first->y());
            break;
        case MODIFY_RESIZE:
            itr->first->resize(itr->second.HasDirectionFlag(MODIFY_DIRECTION_WIDTH) ? parentSize.width() - widgetPosition.x() - itr->second.m_Border.x() : itr->first->width(),
                itr->second.HasDirectionFlag(MODIFY_DIRECTION_HEIGHT) ? parentSize.height() - widgetPosition.y() - itr->second.m_Border.y() : itr->first->height());
            break;
        }
    }
}

void ModifyObject::removeWidget(QWidget *pWidget)
{
    ModifyWidgetMap::iterator itr;
    if (_findWidget(pWidget, itr))
        m_pModifyWidgetMap.erase(itr);
}

void ModifyObject::setWidget(QWidget *pWidget, ModifyState state, QPoint border, uint32 directionFlag)
{
    if (!pWidget)
        return;

    ModifyWidgetMap::iterator itr;
    if (_findWidget(pWidget, itr))
        itr->second = Modifyer(border, state, directionFlag);
    else
        m_pModifyWidgetMap.insert(std::make_pair(pWidget, Modifyer(border, state, directionFlag)));
}