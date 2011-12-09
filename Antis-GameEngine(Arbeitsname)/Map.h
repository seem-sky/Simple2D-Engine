#ifndef MAP_H
#define MAP_H

#include "Logfile.h"
#include "ObjectLayer.h"
#include "Thread.h"

typedef std::vector<std::vector<float>> MapTiles;

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

class MapLoadThread;

class Map
{
public:
    Map(void);
    ~Map(void);
    /*#####
    ## Map System
    #####*/
    void Draw();
    MapLoadResult LoadNewMap(std::string sMapName);
    void UpdateMap(const UINT uiCurTime, const UINT uiDiff);

    const MapInfo* GetMapInfo() { return &m_MapInfo; }
    const std::vector<MapTiles>* GetMapTiles() { return &m_v2MapTiles; }
    //SpriteFiles* GetSpriteFiles() { return m_pSpriteFiles; }

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
    WorldObject* AddNewWorldObject(std::string sFileName, int XPos, int YPos, UINT uiLayerNr);

    /*#####
    ## color
    #####*/
    void SetColorTo(D3DXCOLOR color) { m_MapColor = color; }
    void SetColorTo(float r, float g, float b, float a) { SetColorTo(D3DXCOLOR(r,g,b,a)); }
    D3DXCOLOR GetColor() { return m_MapColor; }

    /*#####
    ## layer
    #####*/
    void AddLayer(Layer *pLayer);
    void EraseLayer(UINT uiLayerNr);
    void EraseLayer(Layer *pLayer);
    void ClearAllLayer();
    Layer* GetLayerAtNr(UINT uiLayerNr);

protected:
    void DrawMap();
    void DrawLayer();

    MapLoadState m_MapLoadState;
    MapInfo m_MapInfo;
    std::vector<MapTiles> m_v2MapTiles;

    D3DXVECTOR3 m_v3Position;
    D3DXCOLOR m_MapColor;

    std::string m_sLogLocationName;

private:
    // string which stores data from file
    std::string m_sMapDataFromFile;
    std::list<std::string> m_TileByRowFromFileList;

    LayerList m_lLayers;

    MapLoadThread *m_pMapLoadThread;
};


struct ObjectReadOut
{
    int m_ObjectID;
    int m_XPos;
    int m_YPos;

    ObjectReadOut()
    {
        m_ObjectID  = 0;
        m_XPos      = 0;
        m_YPos      = 0;
    }
};

// map load object create new thread
class MapLoadThread : public ActiveObject
{
public:
    MapLoadThread(std::string sMapName);

    MapLoadState GetMapLoadState() { return m_MapLoadState; }
    void GetMapInfo(MapInfo &MapInfo, std::vector<MapTiles> &MapTiles, LayerList &LayerList);

protected:
    /*#####
    ## Map Load System
    #####*/
    void Run ();
    // open map file and save data into string
    MapLoadResult LoadDataFromFile(std::string sMapName, std::string *sMapData);
    // load info from data string
    MapLoadResult LoadInfo(std::string *sMapData);
    // load tiles from data string
    MapLoadResult LoadTiles(std::string *sMapData);
    // load layer and objects from data string
    MapLoadResult LoadLayerAndObjects(std::string *sMapData);

private:
    // Logfile
    std::string m_sLogLocationName;

    std::string m_sMapName;
    MapLoadState m_MapLoadState;

    MapInfo m_MapInfo;
    std::vector<MapTiles> m_v2MapTiles;
    LayerList m_lLayers;
};
#endif;