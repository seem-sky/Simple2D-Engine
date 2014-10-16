#include "ObjectMappingMode.h"
#include "moc_ObjectMappingMode.h"
#include "MapEditor.h"
#include "MapViewItem.h"
#include "DelayedDeleteObject.h"
#include "ObjectMappingRevert.h"

namespace MAPPING_MODE
{
    void Object::press(MapEditor& editor, const QPoint& pos, Qt::MouseButton button)
    {
        if (button != Qt::LeftButton)
            return;

        auto selected = editor.scene()->selectedItems();
        if (!selected.isEmpty())
        {
            for (auto pItem : selected)
            {
                if (auto pWO = dynamic_cast<MapViewItem*>(pItem))
                    m_MoveInfos.push_back(pWO->getWorldObjectInfo());
            }
        }
        else if (editor.scene()->itemAt(pos, QTransform()))
            return;
        else if (auto pWorldObject = editor.addWorldObject(m_ID, GEOMETRY::Point<int32>(pos.x(), pos.y()), MAP::MAP_DATA::MapObjectLayer::MIDDLE, m_Direction))
        {
            // add revert
            std::unique_ptr<MAPPING_MODE::REVERT::ObjectAdd> pRevert(new MAPPING_MODE::REVERT::ObjectAdd(pWorldObject->getWorldObjectInfo().getGUID(), editor));
            editor.addRevert(pRevert.release());
        }
    }

    void Object::release(MapEditor& editor, const QPoint& pos, Qt::MouseButton button)
    {
        if (button != Qt::LeftButton)
            return;

        std::unique_ptr<MAP::REVERT::RevertContainer> pRevertContainer(new MAP::REVERT::RevertContainer());
        for (auto& info : m_MoveInfos)
        {
            if (auto pItem = editor.getWorldObject(info.getGUID()))
            {
                if (pItem->getWorldObjectInfo().getPosition() != info.getPosition())
                    pRevertContainer->addRevert(new MAPPING_MODE::REVERT::ObjectModify(info, editor));
            }
        }

        if (!pRevertContainer->isEmpty())
            editor.addRevert(pRevertContainer.release());
        m_MoveInfos.clear();
    }

    void Object::move(MapEditor& editor, const QPoint& pos)
    {
    }

    void Object::copy(const MapEditor& editor)
    {
        m_CopyInfos.clear();
        m_Cut = false;
        for (auto pItem : editor.scene()->selectedItems())
        {
            if (auto pWO = dynamic_cast<MapViewItem*>(pItem))
                m_CopyInfos.push_back(pWO->getWorldObjectInfo());
        }
    }

    void Object::cut(const MapEditor& editor)
    {
        copy(editor);
        if (!m_CopyInfos.empty())
            m_Cut = true;
    }

    void Object::paste(MapEditor& editor, const QPoint& pos)
    {
        std::unique_ptr<MAP::REVERT::RevertContainer> pRevertContainer(new MAP::REVERT::RevertContainer());

        // calculate offset
        GEOMETRY::Point<int32> offset(MATH::maximum<int32>(), MATH::maximum<int32>());
        for (auto& info : m_CopyInfos)
        {
            if (info.getPosition().getX() < offset.getX())
                offset.getX() = info.getPosition().getX();
            if (info.getPosition().getY() < offset.getY())
                offset.getY() = info.getPosition().getY();
        }

        for (auto& info : m_CopyInfos)
        {
            std::unique_ptr<MAPPING_MODE::REVERT::ObjectRemove> pCutRevert;
            if (m_Cut)
            {
                pCutRevert = std::unique_ptr<MAPPING_MODE::REVERT::ObjectRemove>(new MAPPING_MODE::REVERT::ObjectRemove(info, editor));
                editor.removeWorldObject(info.getGUID());
                m_Cut = false;
            }

            if (auto pWorldObject = editor.addWorldObject(info.getID(), GEOMETRY::Point<int32>(pos.x() + info.getPosition().getX() - offset.getX(),
                pos.y() + info.getPosition().getY() - offset.getY()), info.getLayer(), info.getDirection()))
                pRevertContainer->addRevert(new MAPPING_MODE::REVERT::ObjectAdd(pWorldObject->getWorldObjectInfo().getGUID(), editor));

            if (pCutRevert)
                pRevertContainer->addRevert(pCutRevert.release());
        }

        if (!pRevertContainer->isEmpty())
            editor.addRevert(pRevertContainer.release());
    }

    void Object::remove(MapEditor& editor)
    {
        std::unique_ptr<MAP::REVERT::RevertContainer> pRevertContainer(new MAP::REVERT::RevertContainer());

        for (auto pItem : editor.scene()->selectedItems())
        {
            if (auto pWO = dynamic_cast<MapViewItem*>(pItem))
            {
                // add revert
                pRevertContainer->addRevert(new MAPPING_MODE::REVERT::ObjectRemove(pWO->getWorldObjectInfo(), editor));

                // remove object from scene and delete it later
                editor.scene()->removeItem(pItem);
                editor.getMapData().getWorldObjectInfoData().removeWorldObject(pWO->getWorldObjectInfo().getGUID());
                new DelayedDeleteObject<MapViewItem>(pWO);
            }
        }

        if (!pRevertContainer->isEmpty())
            editor.addRevert(pRevertContainer.release());
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
