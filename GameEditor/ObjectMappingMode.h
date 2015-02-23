#ifndef OBJECT_MAPPING_MODE_H
#define OBJECT_MAPPING_MODE_H

#include "MappingModeInterface.h"
#include <WorldObjectInfo.h>
#include <memory>

namespace MAPPING_MODE
{
    class Object : public Interface
    {
        Q_OBJECT
    private:
        void _storeMoveInfoReverts(MapEditor& editor);

    public:
        void press(MapEditor& editor, QMouseEvent* pEvent);
        void release(MapEditor& editor, QMouseEvent* pEvent);
        void move(MapEditor& editor, QMouseEvent* pEvent);

        void keyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);
        void keyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);

        void copy(const MapEditor& editor);
        void cut(const MapEditor& editor);
        void paste(MapEditor& editor, const QPoint& pos);
        void remove(MapEditor& editor);

        void contextMenuRequest(MapEditor& editor, QContextMenuEvent* pEvent);

        Type getModeType() const { return Type::OBJECT_MAPPING; }
        QString getModeName() const { return "object mapping"; }

    public slots:
        void onDirectionChanged(MAP::MAP_DATA::MapDirection dir);
        void onIDChanged(uint32 ID);

    private slots:
        void _onActionCopy(MapEditor& editor, const QPoint& pos);
        void _onActionPaste(MapEditor& editor, const QPoint& pos);
        void _onActionCut(MapEditor& editor, const QPoint& pos);
        void _onActionDelete(MapEditor& editor, const QPoint& pos);

        void _onActionEditLeft(MapEditor& editor, const QPoint& pos);
        void _onActionEditUp(MapEditor& editor, const QPoint& pos);
        void _onActionEditRight(MapEditor& editor, const QPoint& pos);
        void _onActionEditDown(MapEditor& editor, const QPoint& pos);

    private:
        uint32 m_ID = 0;
        MAP::MAP_DATA::MapDirection m_Direction = MAP::MAP_DATA::MapDirection::down;

        bool m_Cut = false;
        std::vector<MAP::MAP_DATA::WorldObjectInfo> m_CopyInfos;
        std::vector<MAP::MAP_DATA::WorldObjectInfo> m_MoveInfos;
    };
}
#endif
