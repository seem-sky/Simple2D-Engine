#include "GameScene.h"
#include <QtGui/QPainter>
#include "QtGlobal.h"
#include "Map.h"
#include "GameWindow.h"
#include "Player_Game.h"

using namespace GAME_LOGIC;
using namespace SCENE;

/*#####
# GameScene
#####*/
GameScene::GameScene(SceneMgr &sceneMgr, const KEY::Keyboard &keyboard) : Scene(sceneMgr, keyboard), m_pDatabaseMgr(new DATABASE::DatabaseMgr())
{
    m_pPlayer = PLAYER::PlayerPtr(new PLAYER::Player_Game(m_Keyboard));
    m_pDatabaseMgr->loadDatabase("projects/untitled/", DATABASE::ALL_DATABASES^DATABASE::TILE_SET_DATABASE);
    m_MapMgr.setDatabaseMgr(m_pDatabaseMgr);
    m_MapMgr.change(m_MapMgr.loadMap(3));
    m_pSceneView = new GameSceneView(this, m_pDatabaseMgr);
    showFPS();
}

void GameScene::update(uint32 uiDiff)
{
    // update player
    m_pPlayer->update(uiDiff);

    // update map
    m_MapMgr.updateCurrent(uiDiff);

    // update scene position
    if (m_MapMgr.getCurrent())
    {
        Int32Point pos = m_MapMgr.getCurrent()->getPosition();
        QRectF sceneRect = m_pSceneView->sceneRect();
        sceneRect.setX(pos.x);
        sceneRect.setY(pos.y);
        m_pSceneView->setSceneRect(sceneRect);
    }

    // redraw scene
    m_pSceneView->update();
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
    if (!pScene || !pScene->getCurrentMap())
        return;

    const MAP::MapPtr &pMap = pScene->getCurrentMap();
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