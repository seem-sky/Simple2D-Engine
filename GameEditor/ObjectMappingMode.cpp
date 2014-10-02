#include "ObjectMappingMode.h"
#include "MapViewerScene.h"
#include "moc_ObjectMappingMode.h"

namespace MAPPING_MODE
{
    void Object::press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button)
    {
    }

    void Object::release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button)
    {
        if (!pScene || button != Qt::LeftButton)
            return;

        if (pScene->selectedItems().isEmpty())
            pScene->addWorldObject(m_ID, pos, MAP::MAP_DATA::MapObjectLayer::MIDDLE, m_Direction);
    }

    void Object::move(MapViewerScene* pScene, QPoint pos)
    {
    }

    void Object::copy(MapViewerScene* pScene, QPoint pos)
    {

    }

    void Object::insert(MapViewerScene* pScene, QPoint pos)
    {

    }

    void Object::cutOut(MapViewerScene* pScene, QPoint pos)
    {

    }

    void Object::onDirectionChanged(MAP::MAP_DATA::MapDirection dir)
    {
        m_Direction = dir;
    }

    void Object::onIDChanged(uint32 ID)
    {
        m_ID = ID;
    }
}
