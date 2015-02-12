#ifndef MAP_BINARY_IO_H
#define MAP_BINARY_IO_H

#include "MapDatabase.h"
#include <functional>
#include <QtCore/QDataStream>
#include "Simple2D-Global.h"
#include "Map/ScriptArea/Manager.h"

namespace MAP
{
    class LayerContainer;

    namespace MAP_DATA
    {
        class WorldObjectInfoData;
        class MapData;
    }

    namespace INPUT
    {
        class MapBinaryReader
        {
        private:
            void _readLayer(QDataStream& in, LayerContainer& mapLayer, LayerType layer, uint16 version);
            void _readObjects(QDataStream& in, MAP_DATA::WorldObjectInfoData& data, uint16 version);
            void _readScriptAreas(QDataStream& in, SCRIPT_AREA::Manager& areas, uint16 version);

            // different versions
            void _readLayerV1(QDataStream& in, LayerContainer& mapLayer, LayerType layer);
            void _readLayerV2(QDataStream& in, LayerContainer& mapLayer, LayerType layer);

            void _readObjectsV1(QDataStream& in, MAP_DATA::WorldObjectInfoData& data);

        public:
            void readFile(const QString& file, MAP_DATA::MapData& data);
        };
    }

    namespace OUTPUT
    {
        class MapBinaryWriter
        {
        private:
            void _writeLayer(QDataStream& out, const LayerContainer& mapLayer, LayerType layer);
            void _writeScriptAreas(QDataStream& out, const SCRIPT_AREA::Manager& areas);
            void _writeObjects(QDataStream& out, const MAP_DATA::WorldObjectInfoData& data);

        public:
            void writeFile(const QString& path, const MAP_DATA::MapData& data);
        };
    }
}
#endif