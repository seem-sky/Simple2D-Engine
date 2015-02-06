#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"
#include "MapMgr.h"
#include "DatabaseMgr.h"
#include "WorldObject.h"
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPixmapCache>
#include "Player_Game.h"

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
            MapItem(const MAP::OBJECT::WorldObject* pWorldObject, const DATABASE::DatabaseMgr& DBMgr);

            void syncWithWorldObject();

            QRectF boundingRect() const;
            void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

        private:
            const DATABASE::DatabaseMgr& m_DBMgr;
            const MAP::OBJECT::WorldObject* m_pWorldObject;
            const DATABASE::PROTOTYPE::ANIMATION::AnimationPrototype* m_pCurrentAnimation;
            uint32 m_uiCurrentFrame;
            QString m_PixmapIdentify;
            QRect m_BoundingRect;
            QPoint m_PixmapPos;
        };

        /*#####
        # GameScene
        #####*/
        class GameScene : public Scene
        {
            MapItem* createNewWorldObject(MAP::OBJECT::WorldObject* pObject);

        public:
            GameScene(Game& game);

            void update(uint32 uiDiff);

            inline const PLAYER::GamePlayer& getPlayer() const { return m_Player; }
            inline PLAYER::GamePlayer& getPlayer() { return m_Player; }

            void playerChangesMap(MAP::MapPtr pMap);

        private:
            PLAYER::GamePlayer m_Player;
            MAP::MapMgr m_MapMgr;
            DATABASE::DatabaseMgr m_DatabaseMgr;
        };

        /*#####
        # GameSceneView
        #####*/
        class GameSceneView : public SceneView
        {
        protected:
            virtual void drawBackground(QPainter* painter, const QRectF& rect);
            virtual void drawForeground(QPainter* painter, const QRectF& rect);

            virtual void drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType layer);

        public:
            GameSceneView(GameScene* pScene, const DATABASE::DatabaseMgr& DBMgr);

            inline GameScene* getGameScene() { return dynamic_cast<GameScene*>(m_pScene); }

        private:
            const DATABASE::DatabaseMgr& m_DatabaseMgr;
        };
    }
}
#endif