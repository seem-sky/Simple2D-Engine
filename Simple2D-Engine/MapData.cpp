#include "MapData.h"
#include "MapBinaryIO.h"
#include "Config.h"
#include "DatabaseMgr.h"

using namespace DATABASE::PROTOTYPE::MAP_STRUCTURE;

namespace MAP
{
    namespace MAP_DATA
    {
        MapData::MapData(const DATABASE::DatabaseMgr& DBMgr, uint32 mapID) : m_DBMgr(DBMgr), m_MapID(mapID)
        {
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
                reader.readFile(pMap->getFilePath(), *this);
            }
            catch (const std::ios::failure&) {}
        }

        void MapData::save()
        {
            if (auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(m_MapID))
            {
                OUTPUT::MapBinaryWriter writer;
                writer.writeFile(pMap->getFilePath(), *this);
            }
        }
    }
}
