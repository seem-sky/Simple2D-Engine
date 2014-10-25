#include "ScriptAreaMappingMode.h"
#include "ScriptArea_AreaRect.h"
#include "MapEditor.h"

using namespace MAP::SCRIPT_AREA;

void MAPPING_MODE::ScriptArea::press(MapEditor& editor, QMouseEvent* pEvent)
{
    if (pEvent->button() != Qt::LeftButton)
        return;

    auto pos = editor.mapToScene(pEvent->pos()).toPoint();
    if (auto pItem = editor.scene()->itemAt(pos, QTransform()))
    {
        if (pItem->type() == QGraphicsItem::UserType + 2 ||
            pItem->type() == QGraphicsItem::UserType + 3)
            return;
    }

    auto pItem = editor.addScriptArea(new GEOMETRY::Rectangle<int32>(
        GEOMETRY::Point<int32>(pos.x(), pos.y()), GEOMETRY::Point<int32>(pos.x() + MAP::SCRIPT_AREA::MINIMUM_SIZE, pos.y() + MAP::SCRIPT_AREA::MINIMUM_SIZE)));
}

void MAPPING_MODE::ScriptArea::release(MapEditor& editor, QMouseEvent* pEvent)
{
}

void MAPPING_MODE::ScriptArea::move(MapEditor& editor, QMouseEvent* pEvent)
{
}
