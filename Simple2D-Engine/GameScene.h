#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"
#include "MapMgr.h"
#include "DatabaseMgr.h"
#include "AutoTileCache.h"
#include "TileCache.h"
#include "WorldObject.h"
#include <QtWidgets/QGraphicsItem>

namespace GAME_LOGIC
{
    namespace SCENE
    {
        /*#####
        # MapItem
        #####*/
        class MapItem : public QGraphicsItem
        {
        private:
            bool _animationChanged();

        public:
            MapItem(MAP::OBJECT::WorldObjectPtr pWorldObject, DATABASE::ConstSpriteDatabasePtr pSpriteDB, DATABASE::ConstAnimationDatabasePtr pAnimationDB);

            void syncWithWorldObject();

            QRectF boundingRect() const;
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        private:
            DATABASE::ConstSpriteDatabasePtr m_pSpriteDB;
            DATABASE::ConstAnimationDatabasePtr m_pAnimationDB;
            MAP::OBJECT::WorldObjectPtr m_pWorldObject;
            DATABASE::ConstAnimationPrototypePtr m_pCurrentAnimation;
            uint32 m_uiCurrentFrame;
            QPixmap m_Pixmap;
            QRect m_BoundingRect;
        };

        /*#####
        # GameScene
        #####*/
        class GameScene : public Scene
        {
            MapItem* createNewWorldObject(MAP::OBJECT::WorldObjectPtr pObject);

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