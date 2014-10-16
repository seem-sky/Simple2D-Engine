#include "MapEditor.h"
#include "moc_MapEditor.h"
#include "QtGlobal.h"
#include "MappingObject.h"
#include "DatabaseMgr.h"
#include "MapEditorScene.h"
#include "BrushRevert.h"
#include "WorldObjectInfo.h"
#include "BrushRevert.h"
#include "VisualViewer.h"
#include "MapViewItem.h"
#include "DelayedDeleteObject.h"
#include <QtGui/QCursor>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtGui/QMouseEvent>

/*#####
# MapViewer
#####*/
MapEditor::MapEditor(uint32 mapID, const MappingObject& mappingObject, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : QGraphicsView(pParent),
m_MapData(DBMgr, mapID), m_DBMgr(DBMgr),
m_pActionCopy(new QShortcut(QKeySequence(QKeySequence::Copy), this)), m_pActionPaste(new QShortcut(QKeySequence(QKeySequence::Paste), this)),
m_pActionCut(new QShortcut(QKeySequence(QKeySequence::Cut), this)), m_pActionDelete(new QShortcut(QKeySequence(QKeySequence::Delete), this))
{
    setScene(new MapEditorScene(mappingObject, m_MapData, DBMgr));
    scene()->setParent(this);
    scene()->setBackgroundBrush(QBrush(Qt::cyan));
    setFrameShape(QFrame::NoFrame);

	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    connect(m_pActionCopy, SIGNAL(activated()), this, SLOT(_onActionCopy()));
    connect(m_pActionPaste, SIGNAL(activated()), this, SLOT(_onActionPaste()));
    connect(m_pActionCut, SIGNAL(activated()), this, SLOT(_onActionCut()));
    connect(m_pActionDelete, SIGNAL(activated()), this, SLOT(_onActionDelete()));
}

MapViewItem* MapEditor::_addWorldObject(const DATABASE::PROTOTYPE::WORLD_OBJECT::WorldObjectPrototype* pWorldObject, MAP::MAP_DATA::WorldObjectInfo& info)
{
    // setup viewer
    auto pItem = new MapViewItem(info);
    scene()->addItem(pItem);
    return _setupWorldObject(pWorldObject, pItem, info);
}

MapViewItem* MapEditor::_setupWorldObject(const DATABASE::PROTOTYPE::WORLD_OBJECT::WorldObjectPrototype* pWorldObject,
    MapViewItem* pItem, const MAP::MAP_DATA::WorldObjectInfo& info)
{
    pItem->setWorldObjectBoundingRect(pWorldObject->getBoundingRect());

    VisualViewer viewer;
    viewer.setFrameShape(QFrame::NoFrame);
    viewer.showGrid(false);
    viewer.setDatabaseMgr(&m_DBMgr);
    auto &animationModule = pWorldObject->getAnimationModule();
    for (uint32 i = 0; i < animationModule.getAnimationCount(); ++i)
    {
        auto animationInfo = animationModule.getAnimationInfo(i);
        if (animationInfo.m_AnimationTypeID - 1 == static_cast<uint32>(info.getDirection()))
        {
            viewer.setAnimation(animationInfo);
            break;
        }
    }

    viewer.stopAnimation();
    viewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set transparent background
    viewer.setStyleSheet("background:transparent;");
    viewer.setAttribute(Qt::WA_TranslucentBackground);
    viewer.setWindowFlags(Qt::FramelessWindowHint);

    auto rect = viewer.scene()->itemsBoundingRect();
    viewer.resizeToContent();
    pItem->setPixmap(viewer.grab());

    // setup position
    //pos = _getNearestAvailablePosition(QPoint(pos.x() - viewer.size().width() / 2, pos.y() - viewer.size().height() / 2), pWorldObject->getBoundingRect());
    pItem->setPos(info.getPosition().getX(), info.getPosition().getY());
    return pItem;
}

MapViewItem* MapEditor::addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MAP::MAP_DATA::MapObjectLayer layer, MAP::MAP_DATA::MapDirection direction)
{
    if (auto pWorldObject = m_DBMgr.getWorldObjectDatabase()->getOriginalPrototype(ID))
    {
        // push into ObjectContainer
        auto pInfo = m_MapData.getWorldObjectInfoData().addWorldObject(ID, pos, layer, direction);
        return _addWorldObject(pWorldObject, *pInfo);
    }
    return nullptr;
}

MapViewItem* MapEditor::addWorldObject(const MAP::MAP_DATA::WorldObjectInfo& info)
{
    if (auto pWorldObject = m_DBMgr.getWorldObjectDatabase()->getOriginalPrototype(info.getID()))
    {
        try
        {
            // push into ObjectContainer
            auto pInfo = getMapData().getWorldObjectInfoData().addWorldObject(info);
            return _addWorldObject(pWorldObject, *pInfo);
        }
        catch (const MAP::MAP_DATA::WorldObjectException&) {}
    }
    return nullptr;
}

void MapEditor::setWorldObject(const MAP::MAP_DATA::WorldObjectInfo& info)
{
    if (auto pProto = m_DBMgr.getWorldObjectDatabase()->getOriginalPrototype(info.getID()))
    {
        for (auto pItem : scene()->items())
        {
            if (auto pWorldObject = dynamic_cast<MapViewItem*>(pItem))
            {
                if (pWorldObject->getWorldObjectInfo().getGUID() == info.getGUID())
                    _setupWorldObject(pProto, pWorldObject, info);
            }
        }
    }
}

void MapEditor::removeWorldObject(MAP::MAP_DATA::GUID guid)
{
    if (auto pItem = getWorldObject(guid))
    {
        scene()->removeItem(pItem);
        getMapData().getWorldObjectInfoData().removeWorldObject(guid);
        new DelayedDeleteObject<MapViewItem>(pItem);
    }
}

MapViewItem* MapEditor::getWorldObject(MAP::MAP_DATA::GUID guid)
{
    for (auto pItem : scene()->items())
    {
        if (auto pWorldObject = dynamic_cast<MapViewItem*>(pItem))
        {
            if (pWorldObject->getWorldObjectInfo().getGUID() == guid)
                return pWorldObject;
        }
    }
    return nullptr;
}

void MapEditor::loadMap()
{
    m_MapData.load();
    auto mapSize = m_MapData.getMapLayer().getSize();
    scene()->setSceneRect(0, 0, mapSize.getX()*TILE_SIZE, mapSize.getY()*TILE_SIZE);
    scene()->update();
}

void MapEditor::saveMap()
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
    {
        m_MapData.save();
        clearReverts();
    }
}

void MapEditor::reloadMap()
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
    {
        m_MapData.reload();
        auto mapSize = pScene->getMapData().getMapLayer().getSize();
        pScene->setSceneRect(0, 0, mapSize.getX()*TILE_SIZE, mapSize.getY()*TILE_SIZE);
        pScene->update();
    }
}

uint32 MapEditor::getMapID() const
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
        return pScene->getMapData().getMapID();
    return 0;
}

void MapEditor::setZoom( uint32 zoom )
{
    resetTransform();
    scale(qreal(zoom)/100, qreal(zoom)/100);
}

uint32 MapEditor::getZoom() const
{
    return transform().m11() * 100;       // get zoom
}

void MapEditor::showGrid(bool show)
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
    {
        auto info = pScene->getMapEditorInfo();
        info.m_ShowGrid = show;
        pScene->setMapEditorInfo(info);
    }
}

bool MapEditor::isGridActive() const
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
        return pScene->getMapEditorInfo().m_ShowGrid;
    return true;
}

void MapEditor::setLayerIndex(uint8 layerIndex)
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
    {
        auto info = pScene->getMapEditorInfo();
        info.m_LayerIndex.at(static_cast<std::size_t>(info.m_LayerType)) = layerIndex;
        pScene->setMapEditorInfo(info);
    }
}

uint8 MapEditor::getLayerIndex() const
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
        return pScene->getMapEditorInfo().m_LayerIndex.at(static_cast<std::size_t>(getLayerType()));
    return 0;
}

void MapEditor::setLayerType(MAP::LayerType layerType)
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
    {
        auto info = pScene->getMapEditorInfo();
        info.m_LayerType = layerType;
        pScene->setMapEditorInfo(info);
    }
}

MAP::LayerType MapEditor::getLayerType() const
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
        return pScene->getMapEditorInfo().m_LayerType;
    return MAP::LayerType::LAYER_BACKGROUND;
}

uint8 MapEditor::getMaximumLayerIndex(MAP::LayerType layerType) const
{
    if (auto pScene = dynamic_cast<MapEditorScene*>(scene()))
        return pScene->getMapData().getMapLayer().getLayerSize(layerType);
    return 0;
}

void MapEditor::contextMenuEvent(QContextMenuEvent* pEvent)
{
    if (!scene())
        return;

    auto selected = scene()->selectedItems();
    if (!selected.isEmpty())
    {
        QMenu menu;
        QAction* pCopy(new QAction("copy", &menu));
        QAction* pInsert(new QAction("paste", &menu));
        QAction* pCut(new QAction("cut", &menu));
        QAction* pDelete(new QAction("delete", &menu));
        menu.addAction(pCopy);
        menu.addAction(pInsert);
        menu.addAction(pCut);
        menu.addAction(pDelete);
        connect(pCopy, SIGNAL(triggered()), this, SLOT(_onActionCopy()));
        connect(pInsert, SIGNAL(triggered()), this, SLOT(_onActionPaste()));
        connect(pCut, SIGNAL(triggered()), this, SLOT(_onActionCut()));
        connect(pDelete, SIGNAL(triggered()), this, SLOT(_onActionDelete()));
        menu.exec(pEvent->globalPos());
    }
}

void MapEditor::mousePressEvent(QMouseEvent* pEvent)
{
    QGraphicsView::mousePressEvent(pEvent);
    emit actionMousePress(*this, mapToScene(pEvent->pos()).toPoint(), pEvent->button());
}

void MapEditor::mouseReleaseEvent(QMouseEvent* pEvent)
{
    QGraphicsView::mouseReleaseEvent(pEvent);
    emit actionMouseRelease(*this, mapToScene(pEvent->pos()).toPoint(), pEvent->button());
}

void MapEditor::mouseMoveEvent(QMouseEvent* pEvent)
{
    QGraphicsView::mouseMoveEvent(pEvent);
    emit actionMouseMove(*this, mapToScene(pEvent->pos()).toPoint());
}

void MapEditor::_onActionCopy()
{
    emit actionCopy(*this);
}

void MapEditor::_onActionCut()
{
    emit actionCut(*this);
}

void MapEditor::_onActionPaste()
{
    emit actionPaste(*this, mapToScene(mapFromGlobal(QCursor::pos())).toPoint());
}

void MapEditor::_onActionDelete()
{
    emit actionDelete(*this);
}

void MapEditor::revertLast()
{
    if (!hasChanged())
        return;

    m_Reverts.back()->revert();
    update();
    m_Reverts.pop_back();
    emit changed(m_MapData.getMapID());
}

void MapEditor::clearReverts()
{
    if (!hasChanged())
        return;

    m_Reverts.clear();
    emit changed(m_MapData.getMapID());
}

void MapEditor::addRevert(MAP::REVERT::Interface* pRevert)
{
    m_Reverts.push_back(std::unique_ptr<MAP::REVERT::Interface>(pRevert));
    emit changed(m_MapData.getMapID());
}
