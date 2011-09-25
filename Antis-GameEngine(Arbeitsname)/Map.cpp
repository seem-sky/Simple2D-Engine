#include "Map.h"

Map::Map(void)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Map : ";
    m_pLogfile      = CLogfile::Get();
    m_pMapSpriteFiles = new SpriteFiles();
    m_pMapSpriteFiles->LoadSpriteDataFromFile(SPRITE_TYPE_MAP);

    m_v3Position.x = 0;
    m_v3Position.y = 0;
    m_v3Position.z = 0;
}

Map::~Map(void)
{
}

MapLoadResult Map::LoadNewMap(std::string sMapName)
{
    MapLoadResult result = MapInput::CreateMapFromFile(m_MapInfo, m_v2MapTiles, sMapName);
    if (result == MAP_LOAD_NO_FILE)
        ERROR_LOG(m_sLogLocationName + "Unable to open " + sMapName + ". Check directory or file name");

    else if (result == MAP_LOAD_CORRUPT_FILE)
        ERROR_LOG(m_sLogLocationName + sMapName + " is a corrupt file. Check syntax or rebuild it.");

    else if (result == MAP_LOAD_CORRUPT_FILE)
        BASIC_LOG(m_sLogLocationName + "Successfully load " + sMapName + ".");

    return result;
}
