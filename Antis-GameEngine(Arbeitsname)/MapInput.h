#ifndef MAP_INPUT_H
#define MAP_INPUT_H

#include "fileinput.h"
#include <vector>

typedef std::vector<std::vector<unsigned int>> MapTiles;

// the da struct for maps
struct MapInfo
{
    std::string m_sMapName;
    std::string m_sMapAnnounceName;

    unsigned int m_uiX;
    unsigned int m_uiY;

    MapInfo()
    {
        m_uiX = 0;
        m_uiY = 0;
    }
};

enum MapLoadResult
{
    MAP_LOAD_OK,
    MAP_LOAD_CORRUPT_FILE,
    MAP_LOAD_NO_FILE,
};

class MapInput
{
public:
    static MapLoadResult CreateMapFromFile(MapInfo &MapInfo, MapTiles &vMapTiles, std::string sMapName);
};
#endif;
