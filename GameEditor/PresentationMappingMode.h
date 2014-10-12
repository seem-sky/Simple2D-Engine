#ifndef PRESENTATION_MAPPING_MODE_H
#define PRESENTATION_MAPPING_MODE_H

#include "MappingModeInterface.h"

namespace MAPPING_MODE
{
    class Presentation : public Interface
    {
    public:
        void press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button) {}
        void release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button) {}
        void move(MapViewerScene* pScene, QPoint pos) {}
        void copy(MapViewerScene* pScene, QPoint pos) {}
        void insert(MapViewerScene* pScene, QPoint pos) {}
        void cutOut(MapViewerScene* pScene, QPoint pos) {}
        void keyPress(MapViewerScene* pScene, int32 key) {}

        Type getModeType() const { return Type::PRESENTATION; }

        QString getModeName() const { return "presentation"; }
    };
}
#endif
