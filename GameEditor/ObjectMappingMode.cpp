#include "ObjectMappingMode.h"
#include "MapViewerScene.h"
#include "MapViewItem.h"
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
        m_pWorldObjectInfo.reset();
        auto pSelected = pScene->selectedItems();
        if (pSelected.isEmpty())
            return;

        m_CutOut = false;
        m_pWorldObjectInfo = std::unique_ptr<MAP::MAP_DATA::WorldObjectInfo>(
            new MAP::MAP_DATA::WorldObjectInfo(dynamic_cast<MapViewItem*>(pSelected.first())->getWorldObjectInfo()));
    }

    void Object::insert(MapViewerScene* pScene, QPoint pos)
    {
        if (m_pWorldObjectInfo)
        {
            if (m_CutOut)
            {
                pScene->removeWorldObject(m_pWorldObjectInfo->getGUID());
                m_CutOut = false;
            }
            pScene->addWorldObject(m_pWorldObjectInfo->getID(), pos, m_pWorldObjectInfo->getLayer(), m_pWorldObjectInfo->getDirection());
        }
    }

    void Object::cutOut(MapViewerScene* pScene, QPoint pos)
    {
        copy(pScene, pos);
        if (m_pWorldObjectInfo)
            m_CutOut = true;
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
