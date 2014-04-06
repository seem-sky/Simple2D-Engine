#include "MapData.h"
#include "MapBinaryIO.h"
#include "Config.h"

using namespace MAP::MAP_DATA;
using namespace DATABASE::MAP_STRUCTURE;

MapData::MapData(const DATABASE::DatabaseMgr& DBMgr, uint32 mapID) : m_DBMgr(DBMgr), m_MapID(mapID)
{
    if (!m_MapID)
        assert("mapID can not be 0!");
}

void MapData::reload()
{
    if (auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(m_MapID))
        m_Layer.resize(pMap->getSize(), pMap->getLayerSize(MAP::LayerType::LAYER_FOREGROUND), pMap->getLayerSize(MAP::LayerType::LAYER_BACKGROUND));
}

void MapData::load()
{
    m_Layer.clear();
    auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(m_MapID);
    if (!pMap)
        return;

    m_Layer.resize(pMap->getSize(), pMap->getLayerSize(MAP::LayerType::LAYER_FOREGROUND), pMap->getLayerSize(MAP::LayerType::LAYER_BACKGROUND));

    try
    {
        INPUT::MapBinaryReader reader;
        reader.readFile(getFilePath(*pMap),* this);
    }
    catch (const std::ios::failure&) {}
}

void MapData::save()
{
    if (auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(m_MapID))
    {
        OUTPUT::MapBinaryWriter writer;
        writer.writeFile(getFilePath(*pMap),* this);
    }
}

QString MAP::getFilePath(const DATABASE::MAP_STRUCTURE::MapPrototype& map)
{
    return Config::get()->getProjectDirectory() + MAP_FOLDER + map.getFileName();
}
