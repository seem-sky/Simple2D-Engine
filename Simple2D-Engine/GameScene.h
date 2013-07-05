#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"
#include "MapMgr.h"
#include "DatabaseMgr.h"
#include "AutoTileCache.h"
#include "TileCache.h"

namespace GAME_LOGIC
{
    namespace SCENE
    {
        /*#####
        # GameScene
        #####*/
        class GameScene : public Scene
        {
        public:
            GameScene(SceneMgr &sceneMgr);

            void update(uint32 uiDiff);

            inline const MAP::MapPtr& getCurrentMap() const { return m_MapMgr.getCurrent(); }

        private:
            MAP::MapMgr m_MapMgr;
            DATABASE::DatabaseMgrPtr m_pDatabaseMgr;
        };

        /*#####
        # GameSceneView
        #####*/
        class GameSceneView : public SceneView
        {
        protected:
            virtual void drawBackground(QPainter *painter, const QRectF &rect);
            virtual void drawForeground(QPainter *painter, const QRectF &rect);

            virtual void drawTiles(QPainter *painter, const QRectF &rect, MAP::Layer layer);

        public:
            GameSceneView(GameScene *pScene, DATABASE::ConstDatabaseMgrPtr pDBMgr);

            inline GameScene* getGameScene() { return dynamic_cast<GameScene*>(m_pScene); }

        private:
            DATABASE::ConstDatabaseMgrPtr m_pDatabaseMgr;
            AutoTileCachePtr m_pAutoTileCache;
            TileCachePtr m_pTileCache;
        };
    }
}
#endif