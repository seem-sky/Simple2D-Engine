#ifndef OBJECT_MAPPING_MODE_H
#define OBJECT_MAPPING_MODE_H

#include "MappingModeInterface.h"

class MapEditorModuleWorldObjects;
class MapViewItem;

namespace MAPPING_MODE
{
    class Object : public Interface
    {
    private:
        void _deselectItem();

    public:
        Object(const MapEditorModuleWorldObjects& objectWidget);

        void press(MapViewer* pViewer, const QMouseEvent* pEvent);
        void release(MapViewer* pViewer, const QMouseEvent* pEvent);
        void move(MapViewer* pViewer, const QMouseEvent* pEvent);

        Type getModeType() const { return Type::OBJECT_MAPPING; }
        QString getModeName() const { return "object mapping"; }

    private:
        const MapEditorModuleWorldObjects& m_ObjectWidget;
    };
}
#endif
