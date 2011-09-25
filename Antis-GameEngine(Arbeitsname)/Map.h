#ifndef MAP_H
#define MAP_H

#include "MapInput.h"
#include "Logfile.h"
#include "SpriteFiles.h"
#include <d3dx9.h>

class Map
{
public:
    Map(void);
    ~Map(void);
    MapLoadResult LoadNewMap(std::string sMapName);

    const MapInfo* GetMapInfo() { return &m_MapInfo; }
    const MapTiles* GetMapTiles() { return &m_v2MapTiles; }
    const SpriteFiles* GetSpriteFiles() { return m_pMapSpriteFiles; }

    // position funktions
    float GetPositionX() { return m_v3Position.x; }
    float GetPositionY() { return m_v3Position.y; }
    float GetPositionZ() { return m_v3Position.z; }
    D3DXVECTOR3 GetPosition() { return m_v3Position; }

    void SetPosition(D3DXVECTOR3 v3Pos) { m_v3Position = v3Pos; }

protected:
    MapInfo m_MapInfo;
    MapTiles m_v2MapTiles;
    SpriteFiles *m_pMapSpriteFiles;

    D3DXVECTOR3 m_v3Position;

    CLogfile *m_pLogfile;
    std::string m_sLogLocationName;
    
};
#endif;