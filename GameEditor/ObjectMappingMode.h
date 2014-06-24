#ifndef OBJECT_MAPPING_MODE_H
#define OBJECT_MAPPING_MODE_H

#include "MappingModeInterface.h"

namespace MAPPING_MODE
{
    class Object : public Interface
    {
    public:
        void press(MapViewer* pViewer, const QMouseEvent* pEvent);
        void release(MapViewer* pViewer, const QMouseEvent* pEvent);
        void move(MapViewer* pViewer, const QMouseEvent* pEvent);

        Type getModeType() const { return Type::OBJECT_MAPPING; }
        QString getModeName() const { return "object mapping"; }
    };
}
#endif
