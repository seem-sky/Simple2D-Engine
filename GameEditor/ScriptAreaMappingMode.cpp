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

    Data scriptData(editor.getMapData().getScriptAreaData().getNewGUID(), AREA::Data(AREA::Type::rect, PointVec<int32> {GEOMETRY::Point<int32>(pos.x(), pos.y())}),
        ACTION::Data("test"));
    auto pItem = editor.addScriptArea(scriptData);
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
