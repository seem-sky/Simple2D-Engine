#ifndef PRESENTATION_MAPPING_MODE_H
#define PRESENTATION_MAPPING_MODE_H

#include "MappingModeInterface.h"

namespace MAPPING_MODE
{
    class Presentation : public Interface
    {
    public:
        void press(MapViewer* pViewer, const QMouseEvent* pEvent) {}
        void release(MapViewer* pViewer, const QMouseEvent* pEvent) {}
        void move(MapViewer* pViewer, const QMouseEvent* pEvent) {}

        Type getModeType() const { return Type::PRESENTATION; }

        QString getModeName() const { return "presentation"; }
    };
}
#endif
