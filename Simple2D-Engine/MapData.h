#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "DatabaseMgr.h"
#include "MapLayer.h"

namespace MAP
{
    namespace MAP_DATA
    {
        enum class MapDirection
        {
            UP,
            RIGHT,
            DOWN,
            LEFT
        };

        enum class MapObjectLayer
        {
            BELOW_BACKGROUND,
            LOWER,
            MIDDLE,
            UPPER,
            ABOVE_FOREGROUND
        };
        const uint32 MAX_OBJECT_LAYER = 5;

        // map objects
        struct MapObject
        {
            MapObject() : m_ObjectID(0), m_GUID(0), m_Direction(MapDirection::DOWN), m_Layer(MapObjectLayer::MIDDLE) {}

            bool isEmpty() const { return !m_GUID || !m_ObjectID; }

            uint32 m_ObjectID;
            uint32 m_GUID;
            Int32Point m_Position;
            MapDirection m_Direction;
            MapObjectLayer m_Layer;
        };
        typedef Container<MapObject> MapObjectContainer;

        class MapData
        {
        public:
            MapData(const DATABASE::DatabaseMgr& DBMgr, uint32 mapID);

            inline MapLayer& getMapLayer() { return m_Layer; }
            inline const MapLayer& getMapLayer() const { return m_Layer; }

            inline uint32 getMapID() const { return m_MapID; }

            // clear map and try to load from file
            void load();
            void save();

            // resize map layer to current MapPrototype size
            void reload();

        private:
            const DATABASE::DatabaseMgr& m_DBMgr;
            const uint32 m_MapID;
            MapLayer m_Layer;
            MapObjectContainer m_Objects;
        };
    }

    QString getFilePath(const DATABASE::MAP_STRUCTURE::MapPrototype& map);
}

#endif
