#include "TileTab.h"
#include "Config.h"
#include "moc_TileTab.h"
#include <QTGui/QBitmap>

using namespace DATABASE;

TileTab::TileTab(QWidget* pParent) : QTabWidget(pParent), MapEditorObject()
{}

void TileTab::clearTiles()
{
    for (int i = count()-1; i >= 0; --i)
        ((TileView*)widget(i))->setScene(new QGraphicsScene());
}

void TileTab::fillTiles(QPixmapCotainer &pixmapCache)
{
    // get tile widget
    //TileView *pTileView = static_cast<TileView*>(widget(0));
    //// get autotile widget
    //TileView *pAutoTileView = static_cast<TileView*>(widget(1));
    //if (!m_pTileDB || !pTileView || !pAutoTileView)
    //    return;

    //QPixmapCotainer tileMap, autoTileMap;
    //// add empty pixmap as 0 tile
    //pixmapCache.resize(1, QPixmapPtr(new QPixmap(QSize(TILE_SIZE, TILE_SIZE))));
    //pixmapCache.at(0)->fill();
    //tileMap.insert(std::make_pair(0, pixmapCache.at(0)));
    //for (uint32 i = 0; i < m_pTileDB->getSize(); ++i)
    //{
    //    ConstTilePrototypePtr proto;
    //    if (!m_pTileDB->getItem(i+1, proto))
    //        continue;

    //    QPixmapPtr pixmapPtr(new QPixmap(Config::Get()->getProjectDirectory()+"/Textures/"+proto->getPathName()));
    //    if (!pixmapPtr)
    //        continue;

    //    // set transparency color
    //    if (proto->getTransparencyColor().hasValidColor())
    //    {
    //        int red, green, blue;
    //        proto->getTransparencyColor().getColor(red, green, blue);
    //        pixmapPtr->setMask(pixmapPtr->createMaskFromColor(QColor(red, green, blue)));
    //    }

    //    if (pixmapCache.size() <= proto->getID())
    //        pixmapCache.resize(proto->getID()+1);

    //    pixmapCache.at(proto->getID()) = pixmapPtr;
    //    tileMap.insert(std::make_pair(proto->getID(), pixmapPtr));
    //}

    //// show autoTiles
    //for (uint32 i = 0; i < m_pAutoTileDB->getSize(); ++i)
    //{
    //    ConstAutoTilePrototypePtr proto;
    //    if (!m_pAutoTileDB->getItem(i+1, proto))
    //        continue;

    //    if (pixmapCache.size() > proto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER))
    //        autoTileMap.insert(std::make_pair(proto->getID(), pixmapCache.at(proto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER))));
    //}

    //pTileView->updateTiles(tileMap, QSize(TILE_SIZE, TILE_SIZE), size());
    //pAutoTileView->updateTiles(autoTileMap, QSize(TILE_SIZE, TILE_SIZE), size());
}

void TileTab::tabInserted(int index)
{
    connect(widget(index), SIGNAL(changeTile(const QPoint, const QSize, uint32)), this, SLOT(_changeCurrentTile(const QPoint&, const QSize&, uint32)));
}

void TileTab::tabRemoved(int index)
{
    disconnect(widget(index), SIGNAL(changeTile(const QPoint&, const QSize&, uint32)), this, SLOT(_changeCurrentTile(const QPoint&, const QSize&, uint32)));
}

void TileTab::_changeCurrentTile(const QPoint &pos, const QSize &tileSize, uint32 uiButton)
{
    for (int i = 0; i < count(); ++i)
    {
        TileView *pView = dynamic_cast<TileView*>(widget(i));
        if (!pView)
            continue;

        QGraphicsScene *pScene = pView->scene();
        if (!pScene)
            continue;

        QList<QGraphicsItem *> pixmapList = pScene->items(Qt::DescendingOrder);
        while (pixmapList.count() > 1)
        {
            pScene->removeItem(*pixmapList.begin());
            pixmapList.erase(pixmapList.begin());
        }
    }

    if (TileView *pView = dynamic_cast<TileView*>(currentWidget()))
    {
        if (QGraphicsScene *pScene = pView->scene())
        {
            QPen pen(QColor(221, 191, 21));
            pen.setWidth(4);
            pScene->addRect(pos.x()*tileSize.width(), pos.y()*tileSize.height(), tileSize.width(), tileSize.height(), pen);
        }
        pView->viewport()->update();
    }
}