#include "MapBinaryIO.h"

using namespace DATABASE::MAP_STRUCTURE;

const uint16 CURRENT_VERSION = 2;

/*#####
# MapBinaryReader
#####*/
void INPUT::MapBinaryReader::readFile(const QString &path, MapPrototypePtr pMap)
{
    if (!pMap)
        throw std::invalid_argument("");

    QFile file(path+pMap->getFileName());
    if (file.fileName().indexOf(MAP_FILE_ENDING) >= file.fileName().length())
        throw std::ios::failure("Invalid file type: " + (path+pMap->getFileName()).toStdString());
    else if (!file.open(QIODevice::ReadOnly))
        throw std::ios::failure("Unable to open file " + (path+pMap->getFileName()).toStdString());

    QDataStream in(&file);
    // load file version
    uint16 version;
    in >> version;

    // load layer data
    _readLayer(in, pMap, MAP::LAYER_BACKGROUND, version);
    _readLayer(in, pMap, MAP::LAYER_FOREGROUND, version);

    // load objects
    _readObjects(in, pMap, version);
}

void INPUT::MapBinaryReader::_readLayer(QDataStream &in, MapPrototypePtr pMap, MAP::Layer layer, uint16 version)
{
    switch(version)
    {
    case 1: _readLayerV1(in, pMap, layer); break;
    case 2: _readLayerV2(in, pMap, layer); break;
    default: throw std::ios::failure("No valid file version."); break;
    }
}

void INPUT::MapBinaryReader::_readObjects(QDataStream &in, MapPrototypePtr pMap, uint16 version)
{
    switch(version)
    {
    case 1:
    case 2:
        _readObjectsV1(in, pMap);
        break;
    default: throw std::ios::failure("No valid file version."); break;
    }
}

// Tile = uint32; AutoTile = uint32
void INPUT::MapBinaryReader::_readLayerV1(QDataStream &in, MapPrototypePtr pMap, MAP::Layer layer)
{
    UInt32Point3D pos;
    for (pos.z = 0; pos.z < pMap->getLayerSize(layer); ++pos.z)
    {
        for (pos.y = 0; pos.y < pMap->getSize().y; ++pos.y)
        {
            for (pos.x = 0; pos.x < pMap->getSize().x; ++pos.x)
            {
                uint32 uiTile, uiAutoTile;
                in >> uiTile >> uiAutoTile;
                MapTile tile(uiTile, uiAutoTile);
                pMap->setMapTile(pos, tile, layer);
            }
        }
    }
}

// Tile = uint16; AutoTile = uint8
void INPUT::MapBinaryReader::_readLayerV2(QDataStream &in, MapPrototypePtr pMap, MAP::Layer layer)
{
    UInt32Point3D pos;
    for (pos.z = 0; pos.z < pMap->getLayerSize(layer); ++pos.z)
    {
        for (pos.y = 0; pos.y < pMap->getSize().y; ++pos.y)
        {
            for (pos.x = 0; pos.x < pMap->getSize().x; ++pos.x)
            {
                MapTile tile;
                in >> tile.m_uiTileID >> tile.m_uiAutoTileSetID;
                pMap->setMapTile(pos, tile, layer);
            }
        }
    }
}

void INPUT::MapBinaryReader::_readObjectsV1(QDataStream &in, MapPrototypePtr pMap)
{
    uint32 objectCount = 0;
    in >> objectCount;
    for (uint32 i = 0; i < objectCount; ++i)
    {
        MapObjectPtr pObj(new MapObject());
        uint32 type = 0, layer = 0;
        in >> pObj->m_GUID >> pObj->m_ObjectID >> type >> pObj->m_Position.x >> pObj->m_Position.y >> layer >> pObj->m_GUID;
        pObj->m_Type = static_cast<MAP_OBJECT::ObjectType>(type);
        pObj->m_Layer = static_cast<MapObjectLayer>(layer);
        pMap->addMapObject(pObj);
    }
}

/*#####
# MapBinaryWriter
#####*/
void OUTPUT::MapBinaryWriter::writeFile(const QString &path, ConstMapPrototypePtr pMap)
{
    if (!pMap)
        throw std::invalid_argument("");

    QFile file(path+pMap->getFileName());
    if (!file.open(QIODevice::WriteOnly))
        throw std::ios::failure("Unable to open file: " + (path+pMap->getFileName()).toStdString());

    QDataStream out(&file);
    // store current version
    out << CURRENT_VERSION;

    // store layer data
    _writeLayer(out, pMap, MAP::LAYER_BACKGROUND);
    _writeLayer(out, pMap, MAP::LAYER_FOREGROUND);

    // store objects
    _writeObjects(out, pMap);
}

void OUTPUT::MapBinaryWriter::_writeLayer(QDataStream &out, ConstMapPrototypePtr pMap, MAP::Layer layer)
{
    UInt32Point3D pos;
    for (pos.z = 0; pos.z < pMap->getLayerSize(layer); ++pos.z)
    {
        for (pos.y = 0; pos.y < pMap->getSize().y; ++pos.y)
        {
            for (pos.x = 0; pos.x < pMap->getSize().x; ++pos.x)
            {
                MapTile tile = pMap->getMapTile(pos, layer);
                out << tile.m_uiTileID << tile.m_uiAutoTileSetID;
            }
        }
    }
}

void OUTPUT::MapBinaryWriter::_writeObjects(QDataStream &out, ConstMapPrototypePtr pMap)
{
    out << pMap->getMapObjectCount();
    for (uint32 i = 1; i <= pMap->getMapObjectCount(); ++i)
    {
        ConstMapObjectPtr pObj;
        if (pMap->getMapObject(i, pObj) && !pObj->isEmpty())
            out << pObj->m_GUID << pObj->m_ObjectID << pObj->m_Type << pObj->m_Position.x << pObj->m_Position.y << pObj->m_Layer << pObj->m_GUID;
    }
}