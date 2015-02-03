#include "ScriptAreaMappingMode.h"
#include "ScriptArea_AreaRect.h"
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
        if (pItem->type() == QGraphicsItem::UserType + 2 ||
            pItem->type() == QGraphicsItem::UserType + 3)
            return;
    }

    auto pItem = editor.addScriptArea(new GEOMETRY::Rectangle<int32>(
        GEOMETRY::Point<int32>(pos.x(), pos.y()), GEOMETRY::Point<int32>(pos.x() + MAP::SCRIPT_AREA::MINIMUM_SIZE, pos.y() + MAP::SCRIPT_AREA::MINIMUM_SIZE)));
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
