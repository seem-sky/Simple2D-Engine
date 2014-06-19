#include "MappingObject.h"
#include "MapViewer.h"

MappingObject::MappingObject(QWidget* pParent, const BRUSH::MapEditorWidgetBrush* pBrushWidget) : QObject(pParent), m_TileMappingMode(pBrushWidget)
{
    setMappingMode(MAPPING_MODE::Mode::TILE_MAPPING);
}

void MappingObject::setMappingMode(MAPPING_MODE::Mode mode)
{
    switch (mode)
    {
    case MAPPING_MODE::Mode::TILE_MAPPING:
        m_pMappingMode = &m_TileMappingMode;
        break;
    case MAPPING_MODE::Mode::OBJECT_MAPPING:
        break;
    case MAPPING_MODE::Mode::PRESENTATION:
        break;
    }
}

void MappingObject::press(MapViewer* pViewer, const QMouseEvent* pEvent)
{
    m_pMappingMode->press(pViewer, pEvent);
}

void MappingObject::release(MapViewer* pViewer, const QMouseEvent* pEvent)
{
    m_pMappingMode->release(pViewer, pEvent);
}

void MappingObject::move(MapViewer* pViewer, const QMouseEvent* pEvent)
{
    m_pMappingMode->move(pViewer, pEvent);
}
