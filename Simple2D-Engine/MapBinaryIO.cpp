#include "MapBinaryIO.h"
#include <QtCore/QFile>
#include <geometry/Point3D.h>
#include "WorldObjectInfoData.h"
#include "MapData.h"

using namespace MAP;
using namespace DATABASE::PROTOTYPE::MAP_STRUCTURE;

const uint16 CURRENT_VERSION = 3;

/*#####
# MapBinaryReader
#####*/
void INPUT::MapBinaryReader::readFile(const QString& fileName, MAP_DATA::MapData& data)
{
    QFile mapFile(fileName);
    if (!mapFile.open(QIODevice::ReadOnly))
        throw std::ios::failure("Unable to open file " + (fileName).toStdString());

    QDataStream in(&mapFile);
    // load file version
    uint16 version;
    in >> version;
    in.setVersion(version);

    // load layer data
    _readLayer(in, data.getMapLayer(), LayerType::LAYER_BACKGROUND, version);
    _readLayer(in, data.getMapLayer(), LayerType::LAYER_FOREGROUND, version);

    // load script areas
    _readScriptAreas(in, data.getScriptAreaManager(), version);

    // load objects
    _readObjects(in, data.getWorldObjectInfoData(), version);
}

void INPUT::MapBinaryReader::_readScriptAreas(QDataStream& in, SCRIPT_AREA::Manager& areas, uint16 version)
{
    uint32 size;
    in >> size;
    for (uint32 i = 0; i < size; ++i)
    {
        SCRIPT_AREA::Data data;
        in >> data;
        areas.addScriptArea(data);
    }
}

void INPUT::MapBinaryReader::_readLayer(QDataStream& in, LayerContainer& mapLayer, LayerType layer, uint16 version)
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

void INPUT::MapBinaryReader::_readObjects(QDataStream& in, MAP::MAP_DATA::WorldObjectInfoData& data, uint16 version)
{
    switch(version)
    {
    case 1:
    case 2:
    case 3:
        _readObjectsV1(in, data);
        break;
    default: throw std::ios::failure("No valid file version."); break;
    }
}

// Tile = uint32; AutoTile = uint32
void INPUT::MapBinaryReader::_readLayerV1(QDataStream& in, MAP::LayerContainer& mapLayer, LayerType layer)
{
    GEOMETRY::Point3D<uint32> pos;
    for (pos.getZ() = 0; pos.getZ() < mapLayer.getLayerSize(layer); ++pos.getZ())
    {
        for (pos.getY() = 0; pos.getY() < mapLayer.getSize().getY(); ++pos.getY())
        {
            for (pos.getX() = 0; pos.getX() < mapLayer.getSize().getX(); ++pos.getX())
            {
                uint32 uiTile, uiAutoTile;
                in >> uiTile >> uiAutoTile;
                MapTile tile(uiTile, uiAutoTile);
                mapLayer.getLayer(layer, pos.getZ()).setMapTile(pos.toPoint(), tile);
            }
        }
    }
}

// Tile = uint16; AutoTile = uint8
void INPUT::MapBinaryReader::_readLayerV2(QDataStream& in, LayerContainer& mapLayer, LayerType layer)
{
    GEOMETRY::Point3D<uint32> pos;
    for (pos.getZ() = 0; pos.getZ() < mapLayer.getLayerSize(layer); ++pos.getZ())
    {
        for (pos.getY() = 0; pos.getY() < mapLayer.getSize().getY(); ++pos.getY())
        {
            for (pos.getX() = 0; pos.getX() < mapLayer.getSize().getX(); ++pos.getX())
            {
                MapTile tile;
                in >> tile.m_uiTileID >> tile.m_uiAutoTileSetID;
                mapLayer.getLayer(layer, pos.getZ()).setMapTile(pos.toPoint(), tile);
            }
        }
    }
}

void INPUT::MapBinaryReader::_readObjectsV1(QDataStream& in, MAP::MAP_DATA::WorldObjectInfoData& data)
{
    uint32 objectCount = 0;
    in >> objectCount;
    for (uint32 i = 0; i < objectCount; ++i)
    {
        MAP::GUID guid;
        in >> guid;
        MAP::MAP_DATA::WorldObjectInfo info(guid);
        uint32 id = 0;
        uint8 layer = 0, direction = 0;
        GEOMETRY::Point<int32> pos;
        in >> id >> layer >> direction >> pos.getX() >> pos.getY();
        info.setID(id);
        info.setLayer(static_cast<MAP_DATA::MapObjectLayer>(layer));
        info.setDirection(static_cast<MAP_DATA::MapDirection>(direction));
        info.setPosition(pos);
        if (info.isValid())
            data.addWorldObject(info);
    }
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
    out.setVersion(CURRENT_VERSION);

    // store layer data
    _writeLayer(out, data.getMapLayer(), MAP::LayerType::LAYER_BACKGROUND);
    _writeLayer(out, data.getMapLayer(), MAP::LayerType::LAYER_FOREGROUND);

    // store script areas
    _writeScriptAreas(out, data.getScriptAreaManager());

    // store objects
    _writeObjects(out, data.getWorldObjectInfoData());
}

void OUTPUT::MapBinaryWriter::_writeLayer(QDataStream& out, const LayerContainer& mapLayer, LayerType layer)
{
    GEOMETRY::Point3D<uint32> pos;
    for (pos.getZ() = 0; pos.getZ() < mapLayer.getLayerSize(layer); ++pos.getZ())
    {
        for (pos.getY() = 0; pos.getY() < mapLayer.getSize().getY(); ++pos.getY())
        {
            for (pos.getX() = 0; pos.getX() < mapLayer.getSize().getX(); ++pos.getX())
            {
                auto& tile = mapLayer.getLayer(layer, pos.getZ()).getMapTile(pos.toPoint());
                out << tile.getMapTile().m_uiTileID << tile.getMapTile().m_uiAutoTileSetID;
            }
        }
    }
}

void OUTPUT::MapBinaryWriter::_writeScriptAreas(QDataStream& out, const SCRIPT_AREA::Manager& areas)
{
    out << areas.count();
    for (auto& area : areas.getScriptAreas())
        out << area->getData();
}

void OUTPUT::MapBinaryWriter::_writeObjects(QDataStream& out, const MAP::MAP_DATA::WorldObjectInfoData& data)
{
    auto& objects = data.getWorldObjects();
    out << uint32(objects.size());
    for (auto& info : objects)
    {
        if (!info->isValid())
            continue;
        out <<
            info->getGUID() <<
            info->getID() <<
            static_cast<uint8>(info->getLayer()) <<
            static_cast<uint8>(info->getDirection()) <<
            info->getPosition().getX() <<
            info->getPosition().getY();
    }
}