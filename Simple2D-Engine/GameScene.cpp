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
GameScene::GameScene(Game &game) : Scene(game), m_Player(m_MapMgr)
{
    m_pSceneView = new GameSceneView(this, m_DatabaseMgr);
    m_DatabaseMgr.loadDatabase("projects/untitled/", DATABASE::DatabaseMgr::ALL_DATABASES^DATABASE::DatabaseMgr::TILE_SET_DATABASE);
    //m_MapMgr.setDatabaseMgr(m_DatabaseMgr);
    playerChangesMap(m_MapMgr.loadMap(2));
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

MapItem* GameScene::createNewWorldObject(MAP::OBJECT::WorldObject *pObject)
{
    MapItem *pItem = new MapItem(pObject, m_DatabaseMgr);
    pItem->setPos(pObject->getPosition().x, pObject->getPosition().y);
    return pItem;
}

void GameScene::update(uint32 uiDiff)
{
    // update player
    m_Player.update(uiDiff);
    // get screen changes for current map
    if (auto pMap = m_MapMgr.getItem(m_Player.getMapGUID()))
    {
        // add new world objects
        for (auto &obj : pMap->getNewWorldObjects())
            m_pSceneView->addItem(createNewWorldObject(obj));
    }

    // update maps
    m_MapMgr.updateMaps(uiDiff);

    // update camera position
    m_pSceneView->views().first()->centerOn(m_Player.getCamera().getPositionX(), m_Player.getCamera().getPositionY());

    // redraw scene
    m_pSceneView->update();
}

/*#####
# MapItem
#####*/
MapItem::MapItem(const MAP::OBJECT::WorldObject *pWorldObject, const DATABASE::DatabaseMgr &DBMgr) : QGraphicsItem(), m_DBMgr(DBMgr), m_pWorldObject(pWorldObject)
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
        if (m_pCurrentAnimation && m_DBMgr.getSpriteDatabase())
        {
            ObjectAnimationWidget widget(m_pCurrentAnimation, m_DBMgr.getSpriteDatabase(), m_uiCurrentFrame);
            QRect boundingRect = widget.scene()->itemsBoundingRect().toRect();
            widget.resize(boundingRect.width(), boundingRect.height());
            pixmap = widget.grab();
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
        m_PixmapPos.setX(0);
        m_PixmapPos.setY(0);
        // store current animation infos
        if (m_pCurrentAnimation)
        {
            DATABASE::ANIMATION::Frame frame;
            m_pCurrentAnimation->getFrame(m_uiCurrentFrame, frame);
            m_PixmapPos = QPoint(frame.getOffset().x, frame.getOffset().y);
            m_PixmapIdentify = "Animation" + QString::number(m_pCurrentAnimation->getID()) + "/" + QString::number(m_uiCurrentFrame);
        }
        else
            m_PixmapIdentify.clear();
    }
    setPos(m_pWorldObject->getPosition().x, m_pWorldObject->getPosition().y);
    setZValue(pos().y()+m_BoundingRect.height());
}

/*#####
# GameSceneView
#####*/
GameSceneView::GameSceneView(GameScene *pScene, const DATABASE::DatabaseMgr &DBMgr) : SceneView(pScene), m_DatabaseMgr(DBMgr),
    m_TileCache(m_DatabaseMgr), m_AutoTileCache(m_TileCache)
{}

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
    auto pMap = player.getMap();
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
                if (tileObj.isEmpty())
                    continue;
                if (!tileObj.isAutoTile())
                {
                    if (auto pPixmap = m_TileCache.getItem(tileObj.m_uiTileID))
                        painter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                }
                else
                {
                    if (auto pAutoTile = m_AutoTileCache.getItem(tileObj.m_uiAutoTileSetID))
                    {
                        const QPixmap *pPixmap(nullptr);
                        if (pAutoTile->getPixmap(static_cast<DATABASE::AUTO_TILE::AUTO_TILE_INDEX>(tileObj.m_uiTileID), pPixmap))
                            painter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                    }
                }
            }
        }
    }
}