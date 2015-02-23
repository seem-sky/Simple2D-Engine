#ifndef SCRIPT_AREA_MAPPING_MODE_H
#define SCRIPT_AREA_MAPPING_MODE_H

#include "MappingModeInterface.h"
#include "ScriptAreaItem.h"
#include <map/ScriptArea/Data.h>

namespace MAPPING_MODE
{
    class ScriptArea : public Interface
    {
    public:
        void press(MapEditor& editor, QMouseEvent* pEvent);
        void release(MapEditor& editor, QMouseEvent* pEvent);
        void move(MapEditor& editor, QMouseEvent* pEvent);

        void keyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) {}
        void keyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) {}

        void copy(const MapEditor& editor);
        void cut(const MapEditor& editor) {}
        void paste(MapEditor& editor, const QPoint& pos);
        void remove(MapEditor& editor);

        void contextMenuRequest(MapEditor& editor, QContextMenuEvent* pEvent) {}

        Type getModeType() const { return Type::SCRIPT_AREA_MAPPING; }
        QString getModeName() const { return "script area mapping"; }

    private:
        std::vector<std::pair<MAP::SCRIPT_AREA::AREA::Data, MAP::SCRIPT_AREA::ACTION::Data>> m_CopyData;
        GEOMETRY::Point<int32> m_Offset;
    };
}
#endif
