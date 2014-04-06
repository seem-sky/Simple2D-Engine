#include "MapBinaryIO.h"
#include <QtCore/QFile>

using namespace MAP;

const uint16 CURRENT_VERSION = 3;

/*#####
# MapBinaryReader
#####*/
void INPUT::MapBinaryReader::readFile(const QString& fileName, MAP::MAP_DATA::MapData& data)
{
    QFile mapFile(fileName);
    if (!mapFile.open(QIODevice::ReadOnly))
        throw std::ios::failure("Unable to open file " + (fileName).toStdString());

    QDataStream in(&mapFile);
    // load file version
    uint16 version;
    in >> version;

    // load layer data
    _readLayer(in, data.getMapLayer(), MAP::LayerType::LAYER_BACKGROUND, version);
    _readLayer(in, data.getMapLayer(), MAP::LayerType::LAYER_FOREGROUND, version);

    //// load objects
    //_readObjects(in, pMap, version);
}

void INPUT::MapBinaryReader::_readLayer(QDataStream& in, MAP::LayerContainer& mapLayer, MAP::LayerType layer, uint16 version)
{
    switch(version)
    {
    case 1: _readLayerV1(in, mapLayer, layer); break;
    case 2:
    case 3:
        _readLayerV2(in, mapLayer, layer); break;
    default: throw std::ios::failure("No valid file version."); break;
    }
}

void INPUT::MapBinaryReader::_readObjects(QDataStream& in, DATABASE::MAP_STRUCTURE::MapPrototype* pMap, uint16 version)
{
    switch(version)
    {
    case 1:
    case 2:
        _readObjectsV1(in, pMap);
    case 3:
        _readObjectsV2(in, pMap);
        break;
    default: throw std::ios::failure("No valid file version."); break;
    }
}

// Tile = uint32; AutoTile = uint32
void INPUT::MapBinaryReader::_readLayerV1(QDataStream& in, MAP::LayerContainer& mapLayer, MAP::LayerType layer)
{
    UInt32Point3D pos;
    for (pos.z = 0; pos.z < mapLayer.getLayerSize(layer); ++pos.z)
    {
        for (pos.y = 0; pos.y < mapLayer.getSize().y; ++pos.y)
        {
            for (pos.x = 0; pos.x < mapLayer.getSize().x; ++pos.x)
            {
                uint32 uiTile, uiAutoTile;
                in >> uiTile >> uiAutoTile;
                MapTile tile(uiTile, uiAutoTile);
                mapLayer.setMapTile(pos, layer, tile);
            }
        }
    }
}

// Tile = uint16; AutoTile = uint8
void INPUT::MapBinaryReader::_readLayerV2(QDataStream& in, MAP::LayerContainer& mapLayer, MAP::LayerType layer)
{
    UInt32Point3D pos;
    for (pos.z = 0; pos.z < mapLayer.getLayerSize(layer); ++pos.z)
    {
        for (pos.y = 0; pos.y < mapLayer.getSize().y; ++pos.y)
        {
            for (pos.x = 0; pos.x < mapLayer.getSize().x; ++pos.x)
            {
                MapTile tile;
                in >> tile.m_uiTileID >> tile.m_uiAutoTileSetID;
                mapLayer.setMapTile(pos, layer, tile);
            }
        }
    }
}

void INPUT::MapBinaryReader::_readObjectsV1(QDataStream& in, DATABASE::MAP_STRUCTURE::MapPrototype* pMap)
{
    // ToDo: add fill into ObjectContainer
    //uint32 objectCount = 0;
    //in >> objectCount;
    //for (uint32 i = 0; i < objectCount; ++i)
    //{
    //    auto pObj = new MapObject();
    //    uint32 type = 0, layer = 0;
    //    in >> pObj->m_GUID >> pObj->m_ObjectID >> type >> pObj->m_Position.x >> pObj->m_Position.y >> layer >> pObj->m_GUID;
    //    pObj->m_Type = static_cast<MAP_OBJECT::ObjectType>(type);
    //    pObj->m_Layer = static_cast<MapObjectLayer>(layer);
    //    pMap->addMapObject(pObj);
    //}
}

// without world_object type (there is only one type left)
void INPUT::MapBinaryReader::_readObjectsV2(QDataStream& in, DATABASE::MAP_STRUCTURE::MapPrototype* pMap)
{
    // ToDo: add fill into ObjectContainer
    //uint32 objectCount = 0;
    //in >> objectCount;
    //for (uint32 i = 0; i < objectCount; ++i)
    //{
    //    auto pObj = new MapObject();
    //    uint32 type = 0, layer = 0;
    //    in >> pObj->m_GUID >> pObj->m_ObjectID >> type >> pObj->m_Position.x >> pObj->m_Position.y >> layer >> pObj->m_GUID;
    //    pObj->m_Type = static_cast<MAP_OBJECT::ObjectType>(type);
    //    pObj->m_Layer = static_cast<MapObjectLayer>(layer);
    //    pMap->addMapObject(pObj);
    //}
}

/*#####
# MapBinaryWriter
#####*/
void OUTPUT::MapBinaryWriter::writeFile(const QString& fileName, const MAP_DATA::MapData& data)
{
    QFile mapFile(fileName);
    if (!mapFile.open(QIODevice::WriteOnly))
        throw std::ios::failure("Unable to open file: " + (fileName).toStdString());

    QDataStream out(&mapFile);
    // store current version
    out << CURRENT_VERSION;

    // store layer data
    _writeLayer(out, data.getMapLayer(), MAP::LayerType::LAYER_BACKGROUND);
    _writeLayer(out, data.getMapLayer(), MAP::LayerType::LAYER_FOREGROUND);

    //// store objects
    //_writeObjects(out, pMap);
}

void OUTPUT::MapBinaryWriter::_writeLayer(QDataStream& out, const LayerContainer& mapLayer, MAP::LayerType layer)
{
    UInt32Point3D pos;
    for (pos.z = 0; pos.z < mapLayer.getLayerSize(layer); ++pos.z)
    {
        for (pos.y = 0; pos.y < mapLayer.getSize().y; ++pos.y)
        {
            for (pos.x = 0; pos.x < mapLayer.getSize().x; ++pos.x)
            {
                auto& tile = mapLayer.getMapTile(pos, layer);
                out << tile.m_uiTileID << tile.m_uiAutoTileSetID;
            }
        }
    }
}

void OUTPUT::MapBinaryWriter::_writeObjects(QDataStream& out, const DATABASE::MAP_STRUCTURE::MapPrototype* pMap)
{
    out << pMap->getMapObjectCount();
    for (uint32 i = 1; i <= pMap->getMapObjectCount(); ++i)
    {
        auto pObj = pMap->getMapObject(i);
        if (pObj && !pObj->isEmpty())
            out << pObj->m_GUID << pObj->m_ObjectID << pObj->m_Position.x << pObj->m_Position.y << pObj->m_Layer << pObj->m_GUID;
    }
}