#ifndef MAPPING_OBJECT_H
#define MAPPING_OBJECT_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>
#include "TileMappingMode.h"

class MapViewer;
class MappingObject : public QObject
{
public:
    MappingObject(QWidget* pParent, const BRUSH::MapEditorWidgetBrush* pBrushWidget);

    void setMappingMode(MAPPING_MODE::Mode mode);
    inline MAPPING_MODE::Mode getMappingModeID() const { return m_pMappingMode->getModeID(); }

    void press(MapViewer* pViewer, const QMouseEvent* pEvent);
    void release(MapViewer* pViewer, const QMouseEvent* pEvent);
    void move(MapViewer* pViewer, const QMouseEvent* pEvent);

private:
    MAPPING_MODE::Interface* m_pMappingMode = nullptr;
    MAPPING_MODE::Tile m_TileMappingMode;
};
#endif
