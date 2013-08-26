#include "GameScene.h"
#include <QtGui/QPainter>
#include <QtWidgets/QScrollBar>
#include "QtGlobal.h"
#include "Map.h"
#include "GameWindow.h"
#include "ObjectAnimationWidget.h"
#include "Game.h"

using namespace GAME_LOGIC;
using namespace SCENE;

/*#####
# GameScene
#####*/
GameScene::GameScene(Game &game) : Scene(game), m_pDatabaseMgr(new DATABASE::DatabaseMgr()), m_Player(m_MapMgr)
{
    m_pSceneView = new GameSceneView(this, m_pDatabaseMgr);
    m_pDatabaseMgr->loadDatabase("projects/untitled/", DATABASE::ALL_DATABASES^DATABASE::TILE_SET_DATABASE);
    m_MapMgr.setDatabaseMgr(m_pDatabaseMgr);
    playerChangesMap(m_MapMgr.loadMap(3));
    showFPS();

    // player key binds
    m_Player.addKeyBind(KEY::KEY_F11, std::bind(&Game::toggleFullScreen, &m_Game), 500);
}

void GameScene::playerChangesMap(MAP::MapPtr pMap)
{
    if (pMap)
    {
        m_Player.setMapGUID(pMap->getGUID());
        m_pSceneView->setSceneRect(0,0,pMap->getWidth()*TILE_SIZE, pMap->getHeight()*TILE_SIZE);
    }
    else
    {
        m_Player.setMapGUID(0);
        m_pSceneView->setSceneRect(0,0,0,0);
    }
}

MapItem* GameScene::createNewWorldObject(MAP::OBJECT::WorldObjectPtr pObject)
{
    MapItem *pItem = new MapItem(pObject, m_pDatabaseMgr->getSpriteDatabase(), m_pDatabaseMgr->getAnimationDatabase());
    pItem->setPos(pObject->getPosition().x, pObject->getPosition().y);
    return pItem;
}

void GameScene::update(uint32 uiDiff)
{
    // update player
    m_Player.update(uiDiff);
    // get screen changes for current map
    MAP::MapPtr pMap;
    if (m_MapMgr.getItem(m_Player.getMapGUID(), pMap))
    {
        // add new world objects
        for (auto &obj : pMap->getNewWorldObjects())
            m_pSceneView->addItem(createNewWorldObject(obj));
    }

    // update maps
    m_MapMgr.updateMaps(uiDiff);

    //// update camera position
    m_pSceneView->views().first()->centerOn(m_Player.getCamera().getPositionX(), m_Player.getCamera().getPositionY());

    // redraw scene
    m_pSceneView->update();
}

/*#####
# MapItem
#####*/
MapItem::MapItem(MAP::OBJECT::WorldObjectPtr pWorldObject, DATABASE::ConstSpriteDatabasePtr pSpriteDB, DATABASE::ConstAnimationDatabasePtr pAnimationDB) :
    QGraphicsItem(), m_pSpriteDB(pSpriteDB), m_pAnimationDB(pAnimationDB), m_pWorldObject(pWorldObject)
{}

bool MapItem::_animationChanged()
{
    return m_pCurrentAnimation != m_pWorldObject->getCurrentAnimation() || m_uiCurrentFrame != m_pWorldObject->getCurrentFrame();
}

QRectF MapItem::boundingRect() const
{
    QPixmap pixmap;
    if (QPixmapCache::find(m_PixmapIdentify, pixmap))
        return pixmap.rect();
    return QRectF();
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    syncWithWorldObject();
    QPixmap pixmap;
    if (!QPixmapCache::find(m_PixmapIdentify, pixmap))
    {
        // generate new pixmap
        if (m_pCurrentAnimation)
        {
            if (m_pCurrentAnimation->getID() == 2)
                int x = 0;
            ObjectAnimationWidget widget(m_pCurrentAnimation, m_pSpriteDB, m_uiCurrentFrame);
            QRect boundingRect = widget.scene()->itemsBoundingRect().toRect();
            widget.resize(boundingRect.width(), boundingRect.height());
            widget.setSceneRect(boundingRect);
            pixmap = widget.grab();
            m_PixmapPos = boundingRect.topLeft();
            QPixmapCache::insert(m_PixmapIdentify, pixmap);
        }
    }
    painter->drawPixmap(m_PixmapPos, pixmap);
}

void MapItem::syncWithWorldObject()
{
    // update pixmap if needed
    if (_animationChanged())
    {
        m_pCurrentAnimation = m_pWorldObject->getCurrentAnimation();
        m_uiCurrentFrame = m_pWorldObject->getCurrentFrame();
        m_BoundingRect = QRect();
        if (m_pWorldObject->getCurrentAnimation())
            m_PixmapIdentify = "Animation" + QString::number(m_pCurrentAnimation->getID()) + "/" + QString::number(m_uiCurrentFrame);
        else
            m_PixmapIdentify.clear();
    }
    setPos(m_pWorldObject->getPosition().x, m_pWorldObject->getPosition().y);
    setZValue(pos().y()+m_BoundingRect.y()+m_BoundingRect.height());
}

/*#####
# GameSceneView
#####*/
GameSceneView::GameSceneView(GameScene *pScene, DATABASE::ConstDatabaseMgrPtr pDBMgr) : SceneView(pScene), m_pDatabaseMgr(pDBMgr), m_pTileCache(new TileCache())
{
    m_pTileCache->setTileDB(m_pDatabaseMgr->getTileDatabase());
    m_pAutoTileCache = AutoTileCachePtr(new AutoTileCache(m_pTileCache));
    m_pAutoTileCache->setAutoTileDB(m_pDatabaseMgr->getAutoTileDatabase());
}

void GameSceneView::drawBackground(QPainter *pPainter, const QRectF &rect)
{
    pPainter->fillRect(rect, QColor(0, 0, 0));
    drawTiles(pPainter, rect, MAP::LAYER_BACKGROUND);
}

void GameSceneView::drawForeground(QPainter *painter, const QRectF &rect)
{
    drawTiles(painter, rect, MAP::LAYER_FOREGROUND);
    SceneView::drawForeground(painter, rect);
}

void GameSceneView::drawTiles(QPainter *painter, const QRectF &rect, MAP::Layer layerType)
{
    GameScene *pScene = getGameScene();
    if (!pScene)
        return;

    const PLAYER::GamePlayer &player = pScene->getPlayer();
    MAP::ConstMapPtr pMap = player.getMap();
    if (!pMap)
        return;
    const UInt32Point startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
    const UInt32Point endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, pMap->getWidth()),
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, pMap->getHeight()));
    for (uint32 layer = 0; layer < pMap->getLayerSize(layerType); ++layer)
    {
        for (uint32 x = startTile.x; x < endTile.x; ++x)
        {
            for (uint32 y = startTile.y; y < endTile.y; ++y)
            {
                MAP::MapTile tileObj = pMap->getMapTile(UInt32Point3D(x, y, layer), layerType);
                if (tileObj.isEmpty() || !tileObj.isValid())
                    continue;
                if (!tileObj.isAutoTile())
                {
                    ConstQPixmapPtr pPixmap;
                    if (m_pTileCache->getItem(tileObj.m_uiTileID, pPixmap))
                        painter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                }
                else
                {
                    ConstQPixmapPtr pPixmap;
                    ConstAutoTilePtr pAutoTile;
                    if (m_pAutoTileCache->getItem(tileObj.m_uiAutoTileSetID, pAutoTile) && pAutoTile->getPixmap(static_cast<DATABASE::AUTO_TILE::AUTO_TILE_INDEX>(tileObj.m_uiTileID), pPixmap))
                        painter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                }
            }
        }
    }
}