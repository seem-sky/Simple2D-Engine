#include "MappingObject.h"
#include "MapViewer.h"
#include "TileMappingMode.h"
#include "ObjectMappingMode.h"
#include "PresentationMappingMode.h"

MappingObject::MappingObject(QWidget* pParent, const BRUSH::MapEditorWidgetBrush& brushWidget, const MapEditorModuleWorldObjects& objectWidget)
    : QObject(pParent)
{
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Tile(brushWidget)));
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Object(objectWidget)));
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Presentation()));
    setMappingMode(MAPPING_MODE::Type::TILE_MAPPING);
}

const MAPPING_MODE::Interface* MappingObject::_getMode(MAPPING_MODE::Type mode) const
{
    return m_MappingModes.at(static_cast<std::size_t>(mode)).get();
}

MAPPING_MODE::Interface* MappingObject::_getMode(MAPPING_MODE::Type mode)
{
    return m_MappingModes.at(static_cast<std::size_t>(mode)).get();
}

void MappingObject::setMappingMode(MAPPING_MODE::Type mode)
{
    m_pCurrentMappingMode = nullptr;
    for (auto& pMode : m_MappingModes)
    {
        if (pMode->getModeType() == mode)
            m_pCurrentMappingMode = pMode.get();
    }
}

MAPPING_MODE::Type MappingObject::getMappingModeType() const
{
    return m_pCurrentMappingMode->getModeType();
}

QStringList MappingObject::getMappingModeNames() const
{
    QStringList list;
    for (auto& pMode : m_MappingModes)
        list.push_back(pMode->getModeName());
    return list;
}

void MappingObject::press(MapViewer* pViewer, const QMouseEvent* pEvent)
{
    m_pCurrentMappingMode->press(pViewer, pEvent);
}

void MappingObject::release(MapViewer* pViewer, const QMouseEvent* pEvent)
{
    m_pCurrentMappingMode->release(pViewer, pEvent);
}

void MappingObject::move(MapViewer* pViewer, const QMouseEvent* pEvent)
{
    m_pCurrentMappingMode->move(pViewer, pEvent);
}
