#ifndef MAP_H
#define MAP_H

#include "Logfile.h"
#include "ObjectLayer.h"
#include "Thread.h"
#include "ScriptPoint.h"

typedef std::vector<std::vector<float>> MapTiles;
typedef std::vector<ScriptPoint*> ScriptPointLIST;

// the da struct for maps
struct MapInfo
{
    std::string m_sMapName;
    std::string m_sMapAnnounceName;

    UINT m_uiX;
    UINT m_uiY;

    MapInfo() : m_uiX(0), m_uiY(0) { }
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
    MAP_STATE_DO_SCRIPT_POINTS,
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
    void UpdateMap(const ULONGLONG uiCurTime, const UINT uiDiff);

    const MapInfo* GetMapInfo() { return &m_MapInfo; }
    const std::vector<MapTiles>* GetMapTiles() { return &m_v2MapTiles; }
    const ScriptPointLIST* GetScriptPoints() { return &m_ScriptPoints; }

    bool IsPassable(UINT XPos, UINT YPos, PassabilityFlag MoveDirection);

    /*#####
    ## position funktions
    #####*/
    inline int GetPositionX() { return m_Position.x; }
    inline int GetPositionY() { return m_Position.y; }
    inline Point<int> GetPosition() { return m_Position; }
    inline void SetPosition(Point<int> pos) { m_Position = pos; }
    inline void ChangePosition(Point<int> pos) { m_Position += pos; }

    /*#####
    ## objects
    #####*/
    // add new world object to map
    WorldObject* AddNewWorldObject(UINT uiObjectID, int XPos, int YPos, UINT uiLayerNr);

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
    UINT GetGUIDForNewObject();

    std::string m_sLogLocationName;

private:
    // string which stores data from file
    std::string m_sMapDataFromFile;
    std::list<std::string> m_TileByRowFromFileList;

    MapLoadState m_MapLoadState;
    MapInfo m_MapInfo;
    std::vector<MapTiles> m_v2MapTiles;
    std::map<UINT, WorldObject*> m_WorldObjectLIST;
    ScriptPointLIST m_ScriptPoints;

    Point<int> m_Position;
    D3DXCOLOR m_MapColor;

    LayerList m_lLayers;

    MapLoadThread *m_pMapLoadThread;
};


struct ObjectReadOut
{
    UINT m_ObjectID;
    UINT m_GUID;
    int m_XPos;
    int m_YPos;
    UINT m_uiDirection;
    UINT m_uiWalkmode;

    ObjectReadOut() : m_ObjectID(0), m_XPos(0), m_YPos(0), m_uiDirection(0), m_GUID(0), m_uiWalkmode(0) { }
};

// map load object create new thread
class MapLoadThread : public ActiveObject
{
public:
    MapLoadThread(std::string sMapName);

    MapLoadState GetMapLoadState() { return m_MapLoadState; }
    void GetMapInfo(MapInfo &MapInfo, std::vector<MapTiles> &MapTiles, LayerList &LayerList, std::map<UINT, WorldObject*> &objectList, ScriptPointLIST &scriptPoints);

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
    // load ScriptPoints from data string
    MapLoadResult LoadScriptPoints(std::string *sMapData);

private:
    // Logfile
    std::string m_sLogLocationName;

    std::string m_sMapName;
    MapLoadState m_MapLoadState;

    MapInfo m_MapInfo;
    std::vector<MapTiles> m_v2MapTiles;
    LayerList m_lLayers;
    ScriptPointLIST m_ScriptPoints;

    std::map<UINT, WorldObject*> m_WorldObjectLIST;
};
#endif;