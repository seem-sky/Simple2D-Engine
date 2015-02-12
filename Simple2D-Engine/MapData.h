#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "MapLayer.h"
#include "WorldObjectInfoData.h"
#include "Map/ScriptArea/Manager.h"
#include <QtCore/QString>

namespace DATABASE
{
    class DatabaseMgr;
}

namespace MAP
{
    namespace MAP_DATA
    {
        class MapData
        {
        public:
            MapData(const DATABASE::DatabaseMgr& DBMgr, uint32 mapID);

            inline LayerContainer& getMapLayer() { return m_Layer; }
            inline const LayerContainer& getMapLayer() const { return m_Layer; }

            WorldObjectInfoData& getWorldObjectInfoData() { return m_Objects; }
            const WorldObjectInfoData& getWorldObjectInfoData() const { return m_Objects; }

            SCRIPT_AREA::Manager& getScriptAreaData() { return m_ScriptAreas; }
            const SCRIPT_AREA::Manager& getScriptAreaData() const { return m_ScriptAreas; }

            inline uint32 getMapID() const { return m_MapID; }

            // clear map and try to load from file
            void load();
            void save();

            // resize map layer to current MapPrototype size
            void reload();

        private:
            const DATABASE::DatabaseMgr& m_DBMgr;
            const uint32 m_MapID;
            SCRIPT_AREA::Manager m_ScriptAreas;
            LayerContainer m_Layer;
            WorldObjectInfoData m_Objects;
        };
    }
}
#endif
