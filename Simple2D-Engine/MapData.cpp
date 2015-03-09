#include "MapData.h"
#include "MapBinaryIO.h"
#include "Config.h"
#include "Database/Manager.h"

using namespace database::prototype;

namespace MAP
{
    namespace MAP_DATA
    {
        MapData::MapData(const database::Manager& DBMgr, uint32 mapID) : m_DBMgr(DBMgr), m_MapID(mapID)
        {
        }

        void MapData::reload()
        {
            if (auto pMap = m_DBMgr.getMapDatabase().getPrototype(m_MapID))
                m_Layer.resize(pMap->getSize(), pMap->getLayerCount(MAP::LayerType::LAYER_FOREGROUND), pMap->getLayerCount(MAP::LayerType::LAYER_BACKGROUND));
        }

        void MapData::load()
        {
            m_Layer.clear();
            auto pMap = m_DBMgr.getMapDatabase().getPrototype(m_MapID);
            if (!pMap)
                return;

            m_Layer.resize(pMap->getSize(), pMap->getLayerCount(MAP::LayerType::LAYER_FOREGROUND), pMap->getLayerCount(MAP::LayerType::LAYER_BACKGROUND));

            try
            {
                INPUT::MapBinaryReader reader;
                reader.readFile(Config::get()->getProjectDirectory() + "/maps/" + pMap->getPathName(), *this);
            }
            catch (const std::ios::failure&) {}
        }

        void MapData::save()
        {
            if (auto pMap = m_DBMgr.getMapDatabase().getPrototype(m_MapID))
            {
                OUTPUT::MapBinaryWriter writer;
                writer.writeFile(pMap->getPathName(), *this);
            }
        }
    }
}
