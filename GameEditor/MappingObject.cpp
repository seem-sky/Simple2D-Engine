#include "MappingObject.h"
#include "MapViewer.h"
#include "TileMappingMode.h"
#include "ObjectMappingMode.h"
#include "PresentationMappingMode.h"
#include "moc_MappingObject.h"

MappingObject::MappingObject(QWidget* pParent, const BRUSH::MapEditorWidgetBrush& brushWidget, const MapEditorModuleWorldObjects& objectWidget)
    : QObject(pParent)
{
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Tile(brushWidget)));
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Object()));
    m_MappingModes.push_back(MappingModePtr(new MAPPING_MODE::Presentation()));
    setMappingMode(MAPPING_MODE::Type::TILE_MAPPING);
}

const MAPPING_MODE::Interface* MappingObject::getMappingMode(MAPPING_MODE::Type mode) const
{
    return m_MappingModes.at(static_cast<std::size_t>(mode)).get();
}

MAPPING_MODE::Interface* MappingObject::getMappingMode(MAPPING_MODE::Type mode)
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

void MappingObject::press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button)
{
    m_pCurrentMappingMode->press(pScene, pos, button);
}

void MappingObject::release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button)
{
    m_pCurrentMappingMode->release(pScene, pos, button);
}

void MappingObject::move(MapViewerScene* pScene, QPoint pos)
{
    m_pCurrentMappingMode->move(pScene, pos);
}
