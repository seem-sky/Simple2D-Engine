#include "MapViewer.h"
#include "MapView.h"
#include "moc_MapViewer.h"
#include <QtGui/QGraphicsSceneMouseEvent>
#include "MapObjectDialog.h"
#include "MapIO.h"

using namespace MAP;

/*#####
# MapObjectItem
#####*/
void MapObjectItem::move(int x, int y)
{
    ModifyItem::move(x, y);
    setZValue(this->y() + pixmap().height());
}

void MapObjectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (MapViewScene *pScene = dynamic_cast<MapViewScene*>(scene()))
    {
        if (ConstSharedMapEditorDataPtr pSharedData = pScene->getSharedData())
        {
            MapObjectDialog objectDialog(pSharedData->getWorldObjectDatabase(), pSharedData->getAnimationDatabase(), pSharedData->getSpriteDatabase(), m_pMapObject);
            if (objectDialog.exec())
            {
                // ToDo: change item pixmap
            }
        }
    }
}

/*#####
# MapViewScene
#####*/
MapViewScene::MapViewScene(MapViewer *pParent) : QGraphicsScene(pParent)
{}

ConstSharedMapEditorDataPtr MapViewScene::getSharedData() const
{
    return m_pSharedData;
}

void MapViewScene::drawBackground(QPainter *pPainter, const QRectF &rect)
{
    // fill background with pink color
    pPainter->fillRect(rect, QColor(255, 0, 249, 255));
    if (!parent() || !((MapViewer*)parent())->getMap() || !m_pSharedData)
        return;

    MapPrototypePtr map = ((MapViewer*)parent())->getMap();
    const Point<uint32> startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
    const Point<uint32> endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, map->getSize().x),
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, map->getSize().y));
    uint32 uiActiveLayer = ((MapViewer*)parent())->getCurrentLayer()-1;
    QRectF tileRect(0, 0, TILE_SIZE, TILE_SIZE);
    for (uint32 layer = 0; layer < map->getLayerSize(LAYER_BACKGROUND); ++layer)
    {
        if (m_pSharedData->getMappingMode() == MODE_TILE)
        {
            // draw over lower layer with transparency black
            if (layer == uiActiveLayer)
                pPainter->fillRect(rect, QColor(0, 0, 0, 125));
            pPainter->setOpacity(layer > uiActiveLayer ? 0.5 : 1);
        }

        for (uint32 x = startTile.x; x < endTile.x; ++x)
        {
            for (uint32 y = startTile.y; y < endTile.y; ++y)
            {
                MapTile tileObj = map->getMapTile(Point3D<uint32>(x, y, layer));
                if (tileObj.m_uiTileID == 0 && tileObj.m_uiAutoTileSetID == 0)      // ignore tile ID 0; empty tile
                    continue;
                // is no auto tile
                if (tileObj.m_uiAutoTileSetID == 0)
                {
                    ConstQPixmapPtr pPixmap;
                    if (m_pSharedData->getTileCache()->getItem(tileObj.m_uiTileID, pPixmap))
                        pPainter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                }
                // autotiles
                else
                {
                    ConstQPixmapPtr pPixmap;
                    ConstAutoTilePtr pAutoTile;
                    if (m_pSharedData->getAutoTileCache()->getItem(tileObj.m_uiAutoTileSetID, pAutoTile) &&
                        pAutoTile->getPixmap(static_cast<DATABASE::AutoTilePrototype::AUTO_TILE_INDEX>(tileObj.m_uiTileID), pPixmap))
                        pPainter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                }
            }
        }
        pPainter->save();
    }
}

void MapViewScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    _drawUpperLayer(painter, rect);
    _drawGrid(painter, rect);
}

void MapViewScene::_drawUpperLayer(QPainter *painter, const QRectF &rect)
{

}

void MapViewScene::_drawGrid(QPainter *painter, const QRectF &rect)
{
    if (!parent() || !((MapViewer*)parent())->getShowGrid() || !((MapViewer*)parent())->getMap())
        return;

    MapPrototypePtr map = ((MapViewer*)parent())->getMap();
    const Point<uint32> startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE * TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE * TILE_SIZE);
    const Point<uint32> endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, map->getSize().x) * TILE_SIZE,
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, map->getSize().y) * TILE_SIZE);
    QVector<QPoint> pointPairs;
    for (uint32 x = startTile.x; x < endTile.x; x += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(x, startTile.y));
        pointPairs.push_back(QPoint(x, endTile.y));
    }
    for (uint32 y = startTile.y; y < endTile.y; y += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(startTile.x, y));
        pointPairs.push_back(QPoint(endTile.x, y));
    }
    painter->drawLines(pointPairs);
}

void MapViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    // check parent class
    if (!selectedItems().isEmpty())
    {
        QGraphicsScene::mouseMoveEvent(pEvent);
        return;
    }

    if (MapViewer *pWidget = dynamic_cast<MapViewer*>(parent()))
        emit brushMoved(pWidget, Point3D<uint32>(pEvent->scenePos().x() / TILE_SIZE, pEvent->scenePos().y() / TILE_SIZE, pWidget->getCurrentLayer()-1));
}

void MapViewScene::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    // check parent class and return if it has a selected item
    QGraphicsScene::mousePressEvent(pEvent);
    if (!selectedItems().isEmpty())
        return;

    if (pEvent->button() == Qt::LeftButton || pEvent->button() == Qt::RightButton)
    {
        if (MapViewer *pWidget = dynamic_cast<MapViewer*>(parent()))
            emit brushPressed(pWidget, Point3D<uint32>(pEvent->scenePos().x() / TILE_SIZE, pEvent->scenePos().y() / TILE_SIZE, pWidget->getCurrentLayer()-1), pEvent->button());
    }
}

void MapViewScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    // check parent class
    if (!selectedItems().isEmpty())
    {
        QGraphicsScene::mouseReleaseEvent(pEvent);
        return;
    }

    if (pEvent->button() == Qt::LeftButton || pEvent->button() == Qt::RightButton)
    {
        if (MapViewer *pWidget = dynamic_cast<MapViewer*>(parent()))
            emit brushReleased(pWidget, Point3D<uint32>(pEvent->scenePos().x() / TILE_SIZE, pEvent->scenePos().y() / TILE_SIZE, pWidget->getCurrentLayer()-1), pEvent->button());
    }
}

/*#####
# MapViewer
#####*/
MapViewer::MapViewer(const MapPrototypePtr &map, SharedMapEditorDataPtr pSharedData, QWidget* pParent) : ModifyView(pParent),
m_uiCurrentLayer(1), m_CurZoom(100), m_hasChanges(false), m_showGrid(true), m_pMap(map)
{
    setSharedData(pSharedData);
    MapViewScene *pScene = new MapViewScene(this);
    pScene->setSharedEditorData(pSharedData);
    setScene(pScene);
    viewport()->setCursor(QCursor(Qt::CrossCursor));
    setDragMode(QGraphicsView::NoDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    updateWidget();
}

void MapViewer::updateWidget()
{
    if (m_pMap)
        setSceneRect(0, 0, m_pMap->getSize().x*TILE_SIZE, m_pMap->getSize().y*TILE_SIZE);
    viewport()->update();
}

void MapViewer::setZoom(int32 zoom)
{
    double newZoom = 100.0/m_CurZoom*zoom/100;
    scale(newZoom, newZoom);
    m_CurZoom = zoom;
}

void MapViewer::revertAction()
{
    if (m_Actions.empty() && m_pSharedData)
        return;

    switch (m_pSharedData->getMappingMode())
    {
    case MODE_TILE:
        m_Actions.at(m_Actions.size()-1)->revertMapAction();
        m_Actions.erase(--m_Actions.end());
        changedMap();
        break;
    case MODE_OBJECT:
        break;
    }
}

void MapViewer::addAction(MAP::MapActionPtr pAction)
{
    m_Actions.push_back(pAction);
}

void MapViewer::saveCurrentMap()
{
    if (!m_pMap)
        return;

    Config *pConfig = Config::Get();
    if (!pConfig)
        return;

    MapWriter writer(m_pMap);
    QString filePath;
    if (MapDatabase::getFilePath(m_pMap, filePath, Config::Get()->getProjectDirectory()))
    {
        writer.writeFile(filePath, "Map");
        if (writer.getXMLResult() == XML_IO::DONE)
        {
            m_hasChanges = false;
            updateText();
            m_Actions.clear();
        }
    }
}

void MapViewer::updateText()
{
    if (!m_pMap)
        return;

    QString sTabName(m_pMap->getName());
    if (hasChanged())
        sTabName.append('*');

    emit textUpdated(this, sTabName);
}

void MapViewer::changedMap()
{
    m_hasChanges = true;
    updateWidget();
    updateText();
}

void MapViewer::setObjectsEditable(bool editable)
{
    MapViewScene *pScene = dynamic_cast<MapViewScene*>(scene());
    if (!pScene)
        return;
    QList<QGraphicsItem*> itemList = pScene->items();
    pScene->clearSelection();
    for (QList<QGraphicsItem*>::const_iterator itr = itemList.begin(); itr != itemList.end(); ++itr)
    {
        if (QGraphicsItem *pItem = *itr)
        {
            pItem->setFlag(QGraphicsItem::ItemIsMovable, editable);
            pItem->setFlag(QGraphicsItem::ItemIsSelectable, editable);
            pItem->setFlag(QGraphicsItem::ItemIsFocusable, editable);
        }
    }
}