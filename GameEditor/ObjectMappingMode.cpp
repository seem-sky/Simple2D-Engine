#include "ObjectMappingMode.h"
#include "moc_ObjectMappingMode.h"
#include "MapEditor.h"
#include "WorldObjectItem.h"
#include "DelayedDeleteObject.h"
#include "ObjectMappingRevert.h"

namespace MAPPING_MODE
{
    void Object::_storeMoveInfoReverts(MapEditor& editor)
    {
        std::unique_ptr<MAP::REVERT::RevertContainer> pRevertContainer(new MAP::REVERT::RevertContainer());
        for (auto& info : m_MoveInfos)
        {
            if (auto pItem = editor.getWorldObject(info.getGUID()))
            {
                if (pItem->getWorldObjectInfo().getPosition() != info.getPosition())
                    pRevertContainer->addRevert(new OBJECT::REVERT::Modify(info, editor));
            }
        }
        editor.addRevert(pRevertContainer.release());
        m_MoveInfos.clear();
    }

    void Object::press(MapEditor& editor, QMouseEvent* pEvent)
    {
        if (pEvent->button() != Qt::LeftButton)
            return;

        auto pos = editor.mapToScene(pEvent->pos()).toPoint();
        auto selected = editor.scene()->selectedItems();
        if (!selected.isEmpty())
        {
            for (auto pItem : selected)
            {
                if (auto pWO = dynamic_cast<WorldObjectItem*>(pItem))
                    m_MoveInfos.push_back(pWO->getWorldObjectInfo());
            }
        }
        else if (editor.scene()->itemAt(pos, QTransform()))
            return;
        else if (auto pWorldObject = editor.addWorldObject(m_ID, GEOMETRY::Point<int32>(pos.x(), pos.y()), MAP::MAP_DATA::MapObjectLayer::MIDDLE, m_Direction))
        {
            // add revert
            std::unique_ptr<OBJECT::REVERT::Add> pRevert(new OBJECT::REVERT::Add(pWorldObject->getWorldObjectInfo().getGUID(), editor));
            editor.addRevert(pRevert.release());
        }
    }

    void Object::release(MapEditor& editor, QMouseEvent* pEvent)
    {
        if (pEvent->button() != Qt::LeftButton)
            return;

        _storeMoveInfoReverts(editor);
    }

    void Object::move(MapEditor& editor, QMouseEvent* pEvent)
    {
    }

    void Object::copy(const MapEditor& editor)
    {
        m_CopyInfos.clear();
        m_Cut = false;
        for (auto pItem : editor.scene()->selectedItems())
        {
            if (auto pWO = dynamic_cast<WorldObjectItem*>(pItem))
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
            std::unique_ptr<OBJECT::REVERT::Remove> pCutRevert;
            if (m_Cut)
            {
                pCutRevert = std::unique_ptr<OBJECT::REVERT::Remove>(new OBJECT::REVERT::Remove(info, editor));
                editor.removeWorldObject(info.getGUID());
                m_Cut = false;
            }

            if (auto pWorldObject = editor.addWorldObject(info.getID(), GEOMETRY::Point<int32>(pos.x() + info.getPosition().getX() - offset.getX(),
                pos.y() + info.getPosition().getY() - offset.getY()), info.getLayer(), info.getDirection()))
                pRevertContainer->addRevert(new OBJECT::REVERT::Add(pWorldObject->getWorldObjectInfo().getGUID(), editor));

            if (pCutRevert)
                pRevertContainer->addRevert(pCutRevert.release());
        }
        editor.addRevert(pRevertContainer.release());
    }

    void Object::remove(MapEditor& editor)
    {
        std::unique_ptr<MAP::REVERT::RevertContainer> pRevertContainer(new MAP::REVERT::RevertContainer());

        for (auto pItem : editor.scene()->selectedItems())
        {
            if (auto pWO = dynamic_cast<WorldObjectItem*>(pItem))
            {
                // add revert
                pRevertContainer->addRevert(new OBJECT::REVERT::Remove(pWO->getWorldObjectInfo(), editor));

                // remove object from scene and delete it later
                editor.scene()->removeItem(pItem);
                editor.getMapData().getWorldObjectInfoData().removeWorldObject(pWO->getWorldObjectInfo().getGUID());
                new DelayedDeleteObject<WorldObjectItem>(pWO);
            }
        }
        editor.addRevert(pRevertContainer.release());
    }

    void Object::keyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent)
    {
        switch (pEvent->key())
        {
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
            pEvent->setAccepted(false);
            bool fill = m_MoveInfos.empty();

            for (auto pItem : editor.scene()->selectedItems())
            {
                if (!pEvent->isAutoRepeat())
                {
                    if (auto pWO = dynamic_cast<WorldObjectItem*>(pItem))
                        m_MoveInfos.push_back(pWO->getWorldObjectInfo());
                }

                int32 move = 1;
                if (pEvent->modifiers() & Qt::ShiftModifier)
                    move = MAP::TILE_SIZE;
                switch (pEvent->key())
                {
                case Qt::Key_Left: pItem->moveBy(-move, 0); break;
                case Qt::Key_Right: pItem->moveBy(move, 0); break;
                case Qt::Key_Up: pItem->moveBy(0, -move); break;
                case Qt::Key_Down: pItem->moveBy(0, move); break;
                }
            }
            break;
        }
    }

    void Object::keyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent)
    {
        switch (pEvent->key())
        {
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
            pEvent->setAccepted(false);
            if (!pEvent->isAutoRepeat())
                _storeMoveInfoReverts(editor);
            break;
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
