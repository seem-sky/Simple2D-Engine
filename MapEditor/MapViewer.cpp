#include "MapViewer.h"
#include "MapView.h"
#include "moc_MapViewer.h"
#include "MapEditor.h"
#include "MainWindow.h"
#include <QtGui/QGraphicsSceneMouseEvent>
#include "MapObjectDialog.h"

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
        if (DATABASE::ConstDatabaseMgrPtr pDBMgr = pScene->getDBMgr())
        {
            MapObjectDialog objectDialog(pDBMgr->getWorldObjectDatabase(), pDBMgr->getAnimationDatabase(),
                pDBMgr->getSpriteDatabase(), m_pMapObject);
            objectDialog.exec();
        }
    }
}

/*#####
# MapViewScene
#####*/
MapViewScene::MapViewScene(MapViewWidget *pParent) : QGraphicsScene(pParent)
{
}

void MapViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!parent() || !((MapViewWidget*)parent())->getMap())
        return;

    MapEditor *pEditor = ((MainWindow*)((MapViewWidget*)parent())->window())->getMapEditorWidget();
    if (!pEditor)
        return;
    MapPrototypePtr map = ((MapViewWidget*)parent())->getMap();
    const Point<uint32> startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
    const Point<uint32> endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, map->getSize().x),
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, map->getSize().y));
    uint32 uiActiveLayer = ((MapViewWidget*)parent())->getActiveLayer()-1;
    QRectF tileRect(0, 0, TILE_SIZE, TILE_SIZE);
    for (uint32 layer = 0; layer < map->getSize().z; ++layer)
    {
        // draw black rect over lower layer
        if (layer == uiActiveLayer)
            painter->fillRect(rect, QColor(0, 0, 0, 125));
        qreal opacity = layer > uiActiveLayer ? 0.5 : 1;
        UInt32Set usedTileIDs, usedAutoTileIDs;
        std::vector<std::vector<QPainter::PixmapFragment>> tileFragmentVector;
        std::vector<std::map<QPixmapPtr, std::vector<QPainter::PixmapFragment>>> autoTileFragmentVector;
        for (uint32 x = startTile.x; x < endTile.x; ++x)
        {
            for (uint32 y = startTile.y; y < endTile.y; ++y)
            {
                MapTile tileObj = map->getMapTile(Point3D<uint32>(x, y, layer));
                if (tileObj.m_uiTileID == 0 && tileObj.m_uiAutoTileSetID == 0)      // ignore tile ID 0; transparency tile
                    continue;
                // is no auto tile
                if (tileObj.m_uiAutoTileSetID == 0)
                {
                    if (tileObj.m_uiTileID >= tileFragmentVector.size())
                        tileFragmentVector.resize(tileObj.m_uiTileID+1);
                    tileFragmentVector.at(tileObj.m_uiTileID).push_back(QPainter::PixmapFragment::create(QPointF(x*TILE_SIZE+(TILE_SIZE>>1),
                        y*TILE_SIZE+(TILE_SIZE>>1)), tileRect, 1, 1, 0, opacity));
                    usedTileIDs.insert(tileObj.m_uiTileID);
                }
                // autotiles
                else
                {
                    QPixmapPtr pixmapPtr;
                    AutoTilePixmapsPtr pAutoTile;
                    if (tileObj.m_uiAutoTileSetID >= autoTileFragmentVector.size())
                        autoTileFragmentVector.resize(tileObj.m_uiAutoTileSetID+1);

                    if ((pEditor->getAutoTilePixmaps(tileObj.m_uiAutoTileSetID, pAutoTile) || pEditor->createAutoTilePixmaps(tileObj.m_uiAutoTileSetID, pAutoTile)) &&
                        pAutoTile->getPixmap(static_cast<DATABASE::AutoTilePrototype::AUTO_TILE_INDEX>(tileObj.m_uiTileID), pixmapPtr))
                    {
                        std::map<QPixmapPtr, std::vector<QPainter::PixmapFragment>>::iterator itr =
                            autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).find(pixmapPtr);
                        if (itr == autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).end())
                        {
                            autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).insert(std::make_pair(pixmapPtr, std::vector<QPainter::PixmapFragment>()));
                            itr = autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).find(pixmapPtr);
                        }
                        itr->second.push_back(QPainter::PixmapFragment::create(QPointF(x*TILE_SIZE+(TILE_SIZE>>1), y*TILE_SIZE+(TILE_SIZE>>1)), tileRect, 1, 1, 0, opacity));
                        usedAutoTileIDs.insert(tileObj.m_uiAutoTileSetID);
                    }
                }
            }
        }

        QPixmapPtr pixmapPtr;
        // draw tiles
        for (UInt32Set::const_iterator itr = usedTileIDs.begin(); itr != usedTileIDs.end(); ++itr)
        {
            if (pEditor->getTilePixmap(*itr, pixmapPtr) && pixmapPtr)
                painter->drawPixmapFragments(&tileFragmentVector.at(*itr).at(0), tileFragmentVector.at(*itr).size(), *pixmapPtr.get());
        }
        // draw autoTiles
        for (UInt32Set::const_iterator IDitr = usedAutoTileIDs.begin(); IDitr != usedAutoTileIDs.end(); ++IDitr)
        {
            for (std::map<QPixmapPtr, std::vector<QPainter::PixmapFragment>>::const_iterator itr = autoTileFragmentVector.at(*IDitr).begin();
                itr != autoTileFragmentVector.at(*IDitr).end(); ++itr)
                painter->drawPixmapFragments(&itr->second.at(0), itr->second.size(), *(itr->first.get()));
        }
    }
}

void MapViewScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (!parent() || !((MapViewWidget*)parent())->showGrid() || !((MapViewWidget*)parent())->getMap())
        return;

    MapPrototypePtr map = ((MapViewWidget*)parent())->getMap();
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

    if (MapViewWidget *pWidget = dynamic_cast<MapViewWidget*>(parent()))
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
        if (MapViewWidget *pWidget = dynamic_cast<MapViewWidget*>(parent()))
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
        if (MapViewWidget *pWidget = dynamic_cast<MapViewWidget*>(parent()))
            emit brushReleased(pWidget, Point3D<uint32>(pEvent->scenePos().x() / TILE_SIZE, pEvent->scenePos().y() / TILE_SIZE, pWidget->getCurrentLayer()-1), pEvent->button());
    }
}


/*#####
# MapViewer
#####*/
MapViewer::MapViewer(QWidget *pParent) : ModifyView(pParent)
{}