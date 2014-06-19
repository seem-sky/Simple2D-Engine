#ifndef MAPPING_MODE_INTERFACE_H
#define MAPPING_MODE_INTERFACE_H

#include <QtGui/QMouseEvent>

class MapViewer;

namespace MAPPING_MODE
{
    enum class Mode
    {
        TILE_MAPPING,
        OBJECT_MAPPING,
        PRESENTATION
    };

    class Interface
    {
    public:
        virtual void press(MapViewer* pViewer, const QMouseEvent* pEvent) = 0;
        virtual void release(MapViewer* pViewer, const QMouseEvent* pEvent) = 0;
        virtual void move(MapViewer* pViewer, const QMouseEvent* pEvent) = 0;

        virtual Mode getModeID() const = 0;
    };
}
#endif
