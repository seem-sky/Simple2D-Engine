#include "ObjectMappingMode.h"
#include "MapViewer.h"

namespace MAPPING_MODE
{
    void Object::press(MapViewer* pViewer, const QMouseEvent* pEvent)
    {

    }

    void Object::release(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        if (!pViewer || !pViewer->getScene() || !pEvent)
            return;
    }

    void Object::move(MapViewer* pViewer, const QMouseEvent* pEvent)
    {

    }
}