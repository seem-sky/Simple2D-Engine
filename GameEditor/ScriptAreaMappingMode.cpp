#include "ScriptAreaMappingMode.h"
#include <Map/ScriptArea/ScriptArea.h>
#include "MapEditor.h"
#include "ScriptAreaMappingRevert.h"
#include <DelayedDeleteObject.h>

using namespace MAP::SCRIPT_AREA;

void MAPPING_MODE::ScriptArea::press(MapEditor& editor, QMouseEvent* pEvent)
{
    if (pEvent->button() != Qt::LeftButton)
        return;

    auto pos = editor.mapToScene(pEvent->pos()).toPoint();
    if (auto pItem = editor.scene()->itemAt(pos, QTransform()))
    {
        if (pItem->type() == MAPPING_MODE::ITEM_MOVE_POINT)
            return;
    }

    auto pItem = editor.addScriptArea(AREA::Data(GEOMETRY::Rectangle<int32>(GEOMETRY::Point<int32>(pos.x(), pos.y()), GEOMETRY::Size<int32>())),
        ACTION::Data("test"));
    editor.addRevert(new MAPPING_MODE::SCRIPT_AREA::REVERT::Add(pItem->getScriptArea()->getGUID(), editor));
}

void MAPPING_MODE::ScriptArea::release(MapEditor& editor, QMouseEvent* pEvent)
{
}

void MAPPING_MODE::ScriptArea::move(MapEditor& editor, QMouseEvent* pEvent)
{
}

void MAPPING_MODE::ScriptArea::remove(MapEditor& editor)
{
    std::unique_ptr<MAP::REVERT::RevertContainer> pRevertContainer(new MAP::REVERT::RevertContainer());

    for (auto pItem : editor.scene()->selectedItems())
    {
        if (auto pScript = dynamic_cast<ScriptAreaItem*>(pItem->parentItem()))
        {
            // take from editor and map and add revert
            editor.takeScriptArea(pScript->getScriptArea()->getGUID());
            new DelayedDeleteObject<ScriptAreaItem>(pScript);
            pRevertContainer->addRevert(new MAPPING_MODE::SCRIPT_AREA::REVERT::Remove(pScript->getScriptArea(), editor));
        }
    }
    editor.addRevert(pRevertContainer.release());
}

void MAPPING_MODE::ScriptArea::copy(const MapEditor& editor)
{
    m_CopyData.clear();
    m_Offset = GEOMETRY::Point<int32>::maximum();
    for (auto pItem : editor.scene()->selectedItems())
    {
        if (auto pScript = dynamic_cast<ScriptAreaItem*>(pItem->parentItem()))
        {
            m_CopyData.push_back(std::make_pair(pScript->getScriptArea()->getArea()->getData(), pScript->getScriptArea()->getAction()->getData()));

            // calculate offset
            m_Offset.getX() = std::min(int32(pScript->x()), m_Offset.getX());
            m_Offset.getY() = std::min(int32(pScript->y()), m_Offset.getY());
        }
    }
}

void MAPPING_MODE::ScriptArea::paste(MapEditor& editor, const QPoint& pos)
{
    for (auto& data : m_CopyData)
    {
        if (auto pItem = editor.addScriptArea(data.first, data.second))
            pItem->moveBy(m_Offset.getX(), m_Offset.getY());
    }
}
