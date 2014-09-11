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

        Type getModeType() const { return Type::PRESENTATION; }

        QString getModeName() const { return "presentation"; }
    };
}
#endif
