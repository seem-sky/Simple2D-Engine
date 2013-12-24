#ifndef MAP_BINARY_IO_H
#define MAP_BINARY_IO_H

#include "MapDatabase.h"
#include <functional>
#include <QtCore/QDataStream>
#include "MapData.h"

namespace MAP
{
    namespace INPUT
    {
        class MapBinaryReader
        {
        private:
            void _readLayer(QDataStream &in, MapLayer &mapLayer, Layer layer, uint16 version);
            void _readObjects(QDataStream &in, DATABASE::MAP_STRUCTURE::MapPrototype *pMap, uint16 version);

            // different versions
            void _readLayerV1(QDataStream &in, MapLayer &mapLayer, Layer layer);
            void _readLayerV2(QDataStream &in, MapLayer &mapLayer, Layer layer);
            void _readObjectsV1(QDataStream &in, DATABASE::MAP_STRUCTURE::MapPrototype *pMap);
            void _readInfoV1(QDataStream &in, DATABASE::MAP_STRUCTURE::MapPrototype *pMap);

        public:
            void readFile(const QString &file, MAP_DATA::MapData &data);
        };
    }

    namespace OUTPUT
    {
        class MapBinaryWriter
        {
        private:
            void _writeLayer(QDataStream &out, const MapLayer &mapLayer, Layer layer);
            void _writeObjects(QDataStream &out, const DATABASE::MAP_STRUCTURE::MapPrototype *pMap);

        public:
            void writeFile(const QString &path, const MAP_DATA::MapData &data);
        };
    }
}
#endif