#include "ObjectMappingMode.h"
#include "moc_ObjectMappingMode.h"
#include "MapViewerScene.h"
#include "MapViewItem.h"
#include "DelayedDeleteObject.h"
#include "ObjectMappingRevert.h"

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
        {
            if (auto pWorldObject = pScene->addWorldObject(m_ID, GEOMETRY::Point<int32>(pos.x(), pos.y()), MAP::MAP_DATA::MapObjectLayer::MIDDLE, m_Direction))
            {
                // add revert
                std::unique_ptr<MAPPING_MODE::REVERT::ObjectAdd> pRevert(new MAPPING_MODE::REVERT::ObjectAdd(pWorldObject->getWorldObjectInfo().getGUID(), *pScene));
                pScene->addRevert(pRevert.release());
            }
        }
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
            std::unique_ptr<MAPPING_MODE::REVERT::ObjectAdd> pInsertRevert;
            std::unique_ptr<MAPPING_MODE::REVERT::ObjectRemove> pCutOutRevert;
            if (m_CutOut)
            {
                pCutOutRevert = std::unique_ptr<MAPPING_MODE::REVERT::ObjectRemove>(new MAPPING_MODE::REVERT::ObjectRemove(*m_pWorldObjectInfo, *pScene));
                pScene->removeWorldObject(m_pWorldObjectInfo->getGUID());
            }

            if (auto pWorldObject = pScene->addWorldObject(m_pWorldObjectInfo->getID(), GEOMETRY::Point<int32>(pos.x(), pos.y()), m_pWorldObjectInfo->getLayer(),
                m_pWorldObjectInfo->getDirection()))
            {
                pInsertRevert = std::unique_ptr<MAPPING_MODE::REVERT::ObjectAdd>(new MAPPING_MODE::REVERT::ObjectAdd(pWorldObject->getWorldObjectInfo().getGUID(), *pScene));
                if (!m_CutOut)
                    pScene->addRevert(pInsertRevert.release());
            }

            // add revert
            if (m_CutOut)
            {
                std::unique_ptr<MAP::REVERT::RevertContainer> pRevertContainer(new MAP::REVERT::RevertContainer());
                pRevertContainer->addRevert(pInsertRevert.release());
                pRevertContainer->addRevert(pCutOutRevert.release());
                pScene->addRevert(pRevertContainer.release());
                m_CutOut = false;
            }
        }
    }

    void Object::cutOut(MapViewerScene* pScene, QPoint pos)
    {
        copy(pScene, pos);
        if (m_pWorldObjectInfo)
            m_CutOut = true;
    }

    void Object::keyPress(MapViewerScene* pScene, int32 key)
    {
        auto selectedObjects = pScene->selectedItems();
        for (auto pItem : selectedObjects)
        {
            switch (key)
            {
            case Qt::Key_Delete:
                if (auto pWorldObject = dynamic_cast<MapViewItem*>(pItem))
                {
                    // add revert
                    std::unique_ptr<MAPPING_MODE::REVERT::ObjectRemove> pRevert(new MAPPING_MODE::REVERT::ObjectRemove(pWorldObject->getWorldObjectInfo(), *pScene));
                    pScene->addRevert(pRevert.release());

                    // remove object from scene and delete it later
                    pScene->removeItem(pWorldObject);
                    pScene->getMapData().getWorldObjectInfoData().removeWorldObject(pWorldObject->getWorldObjectInfo().getGUID());
                    new DelayedDeleteObject<MapViewItem>(pWorldObject);
                    break;
                }

            case Qt::Key_Escape:
                pItem->setSelected(false);
                break;

            case Qt::Key_Up:
                pItem->moveBy(0, -1);
                break;
            case Qt::Key_Left:
                pItem->moveBy(-1, 0);
                break;
            case Qt::Key_Down:
                pItem->moveBy(0, 1);
                break;
            case Qt::Key_Right:
                pItem->moveBy(1, 0);
                break;
            default: break;
            }
        }
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
