#ifndef MAP_H
#define MAP_H

#include "MapInput.h"
#include "Logfile.h"
#include "SpriteFiles.h"
#include <d3dx9.h>
#include "fileinput.h"
#include "WorldObject.h"
#include <vector>

typedef std::vector<std::vector<float>> MapTiles;
typedef std::vector<WorldObject*> ObjectList;

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
    MAP_RESULT_NONE,
    MAP_RESULT_DONE,
    MAP_RESULT_OK,
    MAP_RESULT_IN_PROGRESS,
    MAP_RESULT_CORRUPT_FILE,
    MAP_RESULT_NO_FILE,
    MAP_RESULT_FAILED,
};

enum MapLoadState
{
    MAP_STATE_NONE,
    MAP_STATE_DONE,
    MAP_STATE_DO_INFO,
    MAP_STATE_DO_TILES,
    MAP_STATE_DO_OBJECTS,
};

class Map
{
public:
    Map(void);
    ~Map(void);
    MapLoadResult LoadNewMap(std::string sMapName);

    const MapInfo* GetMapInfo() { return &m_MapInfo; }
    const MapTiles* GetMapTiles() { return &m_v2MapTiles; }
    SpriteFiles* GetSpriteFiles() { return m_pSpriteFiles; }

    /*#####
    ## position funktions
    #####*/
    float GetPositionX() { return m_v3Position.x; }
    float GetPositionY() { return m_v3Position.y; }
    float GetPositionZ() { return m_v3Position.z; }
    D3DXVECTOR3 GetPosition() { return m_v3Position; }
    void SetPosition(D3DXVECTOR3 v3Pos) { m_v3Position = v3Pos; }

    /*#####
    ## objects
    #####*/
    // add new world object to map
    WorldObject* AddNewWorldObject(std::string sFileName, int XPos, int YPos);

protected:
    /*#####
    ## Map Load System
    #####*/
    // open map file and save data into string
    MapLoadResult LoadDataFromFile(std::string sMapName);
    // load info from data string
    MapLoadResult LoadInfo();
    // load tiles from data string
    MapLoadResult LoadTiles();

    MapLoadState m_MapLoadState;
    MapInfo m_MapInfo;
    MapTiles m_v2MapTiles;
    SpriteFiles *m_pSpriteFiles;

    D3DXVECTOR3 m_v3Position;

    CLogfile *m_pLogfile;
    std::string m_sLogLocationName;

private:
    // string which stores data from file
    std::string m_sMapDataFromFile;
    std::list<std::string> m_TileByRowFromFileList;

    ObjectList m_lObjects;
};
#endif;