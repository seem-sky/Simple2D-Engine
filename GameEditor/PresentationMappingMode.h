#ifndef PRESENTATION_MAPPING_MODE_H
#define PRESENTATION_MAPPING_MODE_H

#include "MappingModeInterface.h"

namespace MAPPING_MODE
{
    class Presentation : public Interface
    {
    public:
        void press(MapEditor& editor, const QPoint& pos, Qt::MouseButton button) {}
        void release(MapEditor& editor, const QPoint& pos, Qt::MouseButton button) {}
        void move(MapEditor& editor, const QPoint& pos) {}

        void keyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) {}
        void keyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) {}

        void copy(const MapEditor& editor) {}
        void cut(const MapEditor& editor) {}
        void paste(MapEditor& editor, const QPoint& pos) {}
        void remove(MapEditor& editor) {}

        Type getModeType() const { return Type::PRESENTATION; }

        QString getModeName() const { return "presentation"; }
    };
}
#endif
