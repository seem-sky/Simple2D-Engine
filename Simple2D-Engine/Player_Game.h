#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "Player.h"
#include "DynamicObject.h"
#include "Camera.h"
#include "MapMgr.h"

namespace PLAYER
{
    class GamePlayer : public Player
    {
    public:
        GamePlayer(MAP::MapMgr& mapMgr);

        void setControle(const MAP::OBJECT::DynamicObject* pObject);
        const MAP::OBJECT::DynamicObject* getControledObject();
        void releaseControle();

        // keybinds
        //void moveUp();
        //void moveDown();
        //void moveRight();
        //void moveDown();

        // active map
        inline void setMapGUID(uint32 mapGUID) { m_MapGUID = mapGUID; }
        inline uint32 getMapGUID() const { return m_MapGUID; }
        inline uint32 getMapGUID() { return m_MapGUID; }
        inline const MAP::MapMgr& getMapMgr() const { return m_MapMgr; }
        const MAP::Map* getMap() const { return m_MapMgr.getItem(getMapGUID()); }

        // camera
        inline const CAMERA::Camera& getCamera() const { return m_Camera; }
        inline CAMERA::Camera& getCamera() { return m_Camera; }

        void update(uint32 uiDiff);

    private:
        uint32 m_ObjectGUID;
        uint32 m_MapGUID;
        MAP::MapMgr& m_MapMgr;

    protected:
        CAMERA::MapCamera m_Camera;
    };
}
#endif