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
        void _storeMoveoInfoReverts(MapEditor& editor);

    public:
        void press(MapEditor& editor, const QPoint& pos, Qt::MouseButton button);
        void release(MapEditor& editor, const QPoint& pos, Qt::MouseButton button);
        void move(MapEditor& editor, const QPoint& pos);

        void keyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);
        void keyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);

        void copy(const MapEditor& editor);
        void cut(const MapEditor& editor);
        void paste(MapEditor& editor, const QPoint& pos);
        void remove(MapEditor& editor);

        Type getModeType() const { return Type::OBJECT_MAPPING; }
        QString getModeName() const { return "object mapping"; }

    public slots:
        void onDirectionChanged(MAP::MAP_DATA::MapDirection dir);
        void onIDChanged(uint32 ID);

    private:
        uint32 m_ID = 0;
        MAP::MAP_DATA::MapDirection m_Direction = MAP::MAP_DATA::MapDirection::DOWN;

        bool m_Cut = false;
        std::vector<MAP::MAP_DATA::WorldObjectInfo> m_CopyInfos;
        std::vector<MAP::MAP_DATA::WorldObjectInfo> m_MoveInfos;
    };
}
#endif
