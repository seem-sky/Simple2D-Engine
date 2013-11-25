#ifndef MAP_BINARY_IO_H
#define MAP_BINARY_IO_H

#include "MapDatabase.h"
#include <functional>
#include <QtCore/QDataStream>

namespace DATABASE
{
    namespace MAP_STRUCTURE
    {
        namespace INPUT
        {
            class MapBinaryReader
            {
            private:
                void _readLayer(QDataStream &in, MapPrototype *pMap, MAP::Layer layer, uint16 version);
                void _readObjects(QDataStream &in, MapPrototype *pMap, uint16 version);
                void _readInfo(QDataStream &in, MapPrototype *pMap, uint16 version);

                // different versions
                void _readLayerV1(QDataStream &in, MapPrototype *pMap, MAP::Layer layer);
                void _readLayerV2(QDataStream &in, MapPrototype *pMap, MAP::Layer layer);
                void _readObjectsV1(QDataStream &in, MapPrototype *pMap);
                void _readInfoV1(QDataStream &in, MapPrototype *pMap);

            public:
                void readFile(const QString &path, MapPrototype *pMap);
            };
        }

        namespace OUTPUT
        {
            class MapBinaryWriter
            {
            private:
                void _writeLayer(QDataStream &out, const MapPrototype *pMap, MAP::Layer layer);
                void _writeObjects(QDataStream &out, const MapPrototype *pMap);

            public:
                void writeFile(const QString &path, const MapPrototype *pMap);
            };
        }
    }
}
#endif