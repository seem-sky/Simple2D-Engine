#include "ObjectMappingMode.h"
#include "moc_ObjectMappingMode.h"
#include <QtGui/QCursor>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include "MapEditor.h"
#include "WorldObjectItem.h"
#include "DelayedDeleteObject.h"
#include "ObjectMappingRevert.h"
#include "MappingModeContextMenuAction.h"

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
        switch (pEvent->button())
        {
        case Qt::RightButton:
        {
            if (auto pItem = editor.scene()->itemAt(editor.mapToScene(pEvent->pos()), QTransform()))
                pItem->setSelected(true);
            break;
        }
        case Qt::LeftButton:
        {
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
            break;
        }
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
            int32 move = pEvent->modifiers() & Qt::ShiftModifier ? MAP::TILE_SIZE : 1;

            for (auto pItem : editor.scene()->selectedItems())
            {
                if (!pEvent->isAutoRepeat())
                {
                    if (auto pWO = dynamic_cast<WorldObjectItem*>(pItem))
                        m_MoveInfos.push_back(pWO->getWorldObjectInfo());
                }

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

    void Object::contextMenuRequest(MapEditor& editor, QContextMenuEvent* pEvent)
    {
        QMenu menu;
        auto pPaste = addAction(editor, &menu, "paste", pEvent->pos());
        connect(pPaste, SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionPaste(MapEditor&, const QPoint&)));
        pPaste->setEnabled(!m_CopyInfos.empty());

        auto selected = editor.scene()->selectedItems();
        if (!selected.isEmpty())
        {
            connect(addAction(editor, &menu, "copy", pEvent->pos()), SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionCopy(MapEditor&, const QPoint&)));
            connect(addAction(editor, &menu, "cut", pEvent->pos()), SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionCut(MapEditor&, const QPoint&)));
            connect(addAction(editor, &menu, "delete", pEvent->pos()), SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionDelete(MapEditor&, const QPoint&)));

            menu.addSeparator();

            // edit actions
            auto pEdit = menu.addMenu("edit");

            // directions
            auto pDirection = pEdit->addMenu("direction");
            auto pDirectionGroup = new QActionGroup(pDirection);
            std::vector<QAction*> directions{ addAction(editor, pDirection, "down", pEvent->pos(), pDirectionGroup),
                addAction(editor, pDirection, "left", pEvent->pos(), pDirectionGroup),
                addAction(editor, pDirection, "top", pEvent->pos(), pDirectionGroup),
                addAction(editor, pDirection, "right", pEvent->pos(), pDirectionGroup) };
            for (uint32 i = 0; i < directions.size(); ++i)
            {
                directions.at(i)->setCheckable(true);
                switch (static_cast<MAP::MAP_DATA::MapDirection>(i))
                {
                case MAP::MAP_DATA::MapDirection::down:
                    connect(directions.at(i), SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionEditDown(MapEditor&, const QPoint&)));
                    break;
                case MAP::MAP_DATA::MapDirection::left:
                    connect(directions.at(i), SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionEditLeft(MapEditor&, const QPoint&)));
                    break;
                case MAP::MAP_DATA::MapDirection::up:
                    connect(directions.at(i), SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionEditUp(MapEditor&, const QPoint&)));
                    break;
                case MAP::MAP_DATA::MapDirection::right:
                    connect(directions.at(i), SIGNAL(triggered(MapEditor&, const QPoint&)), this, SLOT(_onActionEditRight(MapEditor&, const QPoint&)));
                    break;
                }
            }

            // if all directions are the same, check the action; otherwise leave all unchecked
            auto dir = MAP::MAP_DATA::MapDirection::down;
            bool check = true;
            for (int32 i = 0; i < selected.size(); ++i)
            {
                if (auto pObject = dynamic_cast<WorldObjectItem*>(selected.at(i)))
                {
                    if (!i)
                        dir = pObject->getWorldObjectInfo().getDirection();
                    else if (dir != pObject->getWorldObjectInfo().getDirection())
                    {
                        check = false;
                        break;
                    }
                }
            }
            if (check)
                directions.at(static_cast<uint32>(dir))->setChecked(true);
        }

        menu.exec(pEvent->globalPos());
    }

    void changeObjectDirection(const QList<QGraphicsItem*>& list, MAP::MAP_DATA::MapDirection direction)
    {
        for (auto pItem : list)
        {
            if (auto pObject = dynamic_cast<WorldObjectItem*>(pItem))
            {
                auto info = pObject->getWorldObjectInfo();
                info.setDirection(direction);
                pObject->setWorldObjectInfo(info);
            }
        }
    }

    void Object::_onActionEditLeft(MapEditor& editor, const QPoint& pos)
    {
        changeObjectDirection(editor.scene()->selectedItems(), MAP::MAP_DATA::MapDirection::left);
    }

    void Object::_onActionEditUp(MapEditor& editor, const QPoint& pos)
    {
        changeObjectDirection(editor.scene()->selectedItems(), MAP::MAP_DATA::MapDirection::up);
    }

    void Object::_onActionEditRight(MapEditor& editor, const QPoint& pos)
    {
        changeObjectDirection(editor.scene()->selectedItems(), MAP::MAP_DATA::MapDirection::right);
    }

    void Object::_onActionEditDown(MapEditor& editor, const QPoint& pos)
    {
        changeObjectDirection(editor.scene()->selectedItems(), MAP::MAP_DATA::MapDirection::down);
    }

    void Object::_onActionCopy(MapEditor& editor, const QPoint& pos)
    {
        copy(editor);
    }

    void Object::_onActionCut(MapEditor& editor, const QPoint& pos)
    {
        cut(editor);
    }

    void Object::_onActionPaste(MapEditor& editor, const QPoint& pos)
    {
        paste(editor, pos);
    }

    void Object::_onActionDelete(MapEditor& editor, const QPoint& pos)
    {
        remove(editor);
    }
}
