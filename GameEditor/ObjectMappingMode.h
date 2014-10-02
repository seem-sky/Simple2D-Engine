#ifndef OBJECT_MAPPING_MODE_H
#define OBJECT_MAPPING_MODE_H

#include "MappingModeInterface.h"
#include <WorldObjectInfo.h>

namespace MAPPING_MODE
{
    class Object : public Interface
    {
        Q_OBJECT
    private:
        void _deselectItem();

    public:
        void press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
        void release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
        void move(MapViewerScene* pScene, QPoint pos);
        void copy(MapViewerScene* pScene, QPoint pos);
        void insert(MapViewerScene* pScene, QPoint pos);
        void cutOut(MapViewerScene* pScene, QPoint pos);

        Type getModeType() const { return Type::OBJECT_MAPPING; }
        QString getModeName() const { return "object mapping"; }

    public slots:
        void onDirectionChanged(MAP::MAP_DATA::MapDirection dir);
        void onIDChanged(uint32 ID);

    private:
        uint32 m_ID = 0;
        MAP::MAP_DATA::MapDirection m_Direction = MAP::MAP_DATA::MapDirection::DOWN;
    };
}
#endif
