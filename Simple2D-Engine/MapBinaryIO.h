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
                void _readLayer(QDataStream &in, MapPrototypePtr pMap, MAP::Layer layer, uint16 version);
                void _readObjects(QDataStream &in, MapPrototypePtr pMap, uint16 version);
                void _readInfo(QDataStream &in, MapPrototypePtr pMap, uint16 version);

                // different versions
                void _readLayerV1(QDataStream &in, MapPrototypePtr pMap, MAP::Layer layer);
                void _readLayerV2(QDataStream &in, MapPrototypePtr pMap, MAP::Layer layer);
                void _readObjectsV1(QDataStream &in, MapPrototypePtr pMap);
                void _readInfoV1(QDataStream &in, MapPrototypePtr pMap);

            public:
                void readFile(const QString &path, MapPrototypePtr pMap);
            };
        }

        namespace OUTPUT
        {
            class MapBinaryWriter
            {
            private:
                void _writeLayer(QDataStream &out, ConstMapPrototypePtr pMap, MAP::Layer layer);
                void _writeObjects(QDataStream &out, ConstMapPrototypePtr pMap);

            public:
                void writeFile(const QString &path, ConstMapPrototypePtr pMap);
            };
        }
    }
}
#endif