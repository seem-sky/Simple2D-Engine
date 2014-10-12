#ifndef MAPPING_MODE_INTERFACE_H
#define MAPPING_MODE_INTERFACE_H

#include <QtGui/QMouseEvent>
#include <QtCore/QObject>
#include <Global.h>

class MapViewerScene;

namespace MAPPING_MODE
{
    enum class Type
    {
        TILE_MAPPING,
        OBJECT_MAPPING,
        PRESENTATION
    };

    class Interface : public QObject
    {
    public:
        Interface(QObject* pParent = nullptr) : QObject(pParent) {}

        virtual void press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button) = 0;
        virtual void release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button) = 0;
        virtual void move(MapViewerScene* pScene, QPoint pos) = 0;
        virtual void copy(MapViewerScene* pScene, QPoint pos) = 0;
        virtual void insert(MapViewerScene* pScene, QPoint pos) = 0;
        virtual void cutOut(MapViewerScene* pScene, QPoint pos) = 0;
        virtual void keyPress(MapViewerScene* pScene, int32 key) = 0;

        virtual Type getModeType() const = 0;
        virtual QString getModeName() const = 0;
    };
}
#endif
