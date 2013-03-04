#include "TileTab.h"
#include "Database.h"
#include "Config.h"
#include "moc_TileTab.h"
#include <QTGui/QBitmap>

using namespace DATABASE;

TileTab::TileTab(QWidget* pParent) : QTabWidget(pParent)
{}

void TileTab::clearTiles()
{
    for (int i = count()-1; i >= 0; --i)
        ((TileView*)widget(i))->setScene(new QGraphicsScene());
}

void TileTab::fillTiles(QPixmapPtrVector &pixmapCache)
{
    pixmapCache.clear();
    const SpritePrototypeMap* pTileMap = Database::Get()->GetTexturePrototypes("Tiles");
    // get tile widget
    TileView *pTileView = static_cast<TileView*>(widget(0));
    // get autotile widget
    TileView *pAutoTileView = static_cast<TileView*>(widget(1));
    if (!pTileMap || !pTileView || !pAutoTileView)
        return;

    QPixmapPtrMap tileMap, autoTileMap;
    // add empty pixmap as 0 tile
    pixmapCache.resize(1, QPixmapPtr(new QPixmap(QSize(TILE_SIZE, TILE_SIZE))));
    pixmapCache.at(0)->fill();
    tileMap.insert(std::make_pair(0, pixmapCache.at(0)));
    for (SpritePrototypeMap::const_iterator itr = pTileMap->begin(); itr != pTileMap->end(); ++itr)
    {
        QPixmapPtr pixmapPtr(new QPixmap(QString::fromStdString(Config::Get()->getProjectDirectory()+"/Textures/"+itr->second.GetFilePath())));
        if (!pixmapPtr)
            continue;

        // set transparency color
        if (itr->second.m_TransparencyColor.hasValidColor())
        {
            int red, green, blue;
            itr->second.m_TransparencyColor.getColor(red, green, blue);
            pixmapPtr->setMask(pixmapPtr->createMaskFromColor(QColor(red, green, blue)));
        }

        if (pixmapCache.size() <= itr->first)
            pixmapCache.resize(itr->first+1);

        pixmapCache.at(itr->first) = pixmapPtr;
        if (itr->second.Type.Tile.m_bAutotile)
            autoTileMap.insert(std::make_pair(itr->first, pixmapPtr));
        else
            tileMap.insert(std::make_pair(itr->first, pixmapPtr));
    }
    pTileView->updateTiles(tileMap, QSize(TILE_SIZE, TILE_SIZE), size());
    pAutoTileView->updateTiles(autoTileMap, QSize(TILE_SIZE*3, TILE_SIZE*3), size());
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
    }
}