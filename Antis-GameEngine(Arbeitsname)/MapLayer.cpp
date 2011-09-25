#include "MapLayer.h"
#include "Game.h"

MapLayer::MapLayer(void) : Layer()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "MapLayer : ";
    m_LayerType         = LAYER_TYPE_MAP;
    m_pMap              = new Map();
}

MapLayer::~MapLayer(void)
{
    if (m_pSprite)
        m_pSprite->Release();

    if (m_pMap)
        delete m_pMap;
}

void MapLayer::LoadNewMap(std::string sMapName)
{
    if (!m_pMap)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to find a valid pointer to map. Return without loading new map.");
        return;
    }

    m_pMap->LoadNewMap(sMapName);
}

void MapLayer::Draw()
{
    if (!m_pMap)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to find a valid pointer to map. Return without drawing.");
        return;
    }

    CGame *pGame = CGame::Get();
    if (!pGame)
        return;

    CGameInfo *pGameInfo = pGame->GetGameInfo();
    if (!pGameInfo)
        return;

    // get screen resolution
    unsigned int uiScreenWidth = 0;
    unsigned int uiScreenHeight = 0;
    pGameInfo->GetWindowSize(uiScreenWidth, uiScreenHeight);

    const MapInfo *MapInfo          = m_pMap->GetMapInfo();
    const MapTiles *MapTiles        = m_pMap->GetMapTiles();
    const SpriteFiles *SpriteFiles  = m_pMap->GetSpriteFiles();
    // return if one of the info files are corrupt
    if (!MapInfo || !MapTiles || !SpriteFiles)
        return;
    
    D3DXVECTOR3 vPosition = m_pMap->GetPosition();

    // get maptile size
    UINT uiMapTileSize_X = 0;
    UINT uiMapTileSize_Y = 0;
    pGameInfo->GetMapTileSize(uiMapTileSize_X, uiMapTileSize_Y);

    // get start pos left and up
    unsigned int startposX = 0;
    unsigned int startposY = 0;
    if (vPosition.x < 0)
        startposX = (unsigned int)((-1) * vPosition.x / uiMapTileSize_X);
    if (vPosition.y < 0)
        startposY = (unsigned int)((-1) * vPosition.y / uiMapTileSize_Y);

    // get end pos right and bottom
    unsigned int endposX = 0;
    unsigned int endposY = 0;
    if (vPosition.x + MapInfo->m_uiX * uiMapTileSize_X > uiScreenWidth)
    {
        if (vPosition.x < 0)
            endposX = uiScreenWidth / uiMapTileSize_X + startposX + 1;
        else if (vPosition.x < uiScreenWidth)
            endposX = (unsigned int)((uiScreenWidth - vPosition.x)/ uiMapTileSize_X + startposX + 1);
    }
    else
        endposX = MapInfo->m_uiX;

    if (vPosition.y + MapInfo->m_uiY * uiMapTileSize_Y > uiScreenHeight)
    {
        if (vPosition.y < 0)
            endposY = uiScreenHeight / uiMapTileSize_Y + startposY + 1;
        else if (vPosition.y < uiScreenHeight)
            endposY = (unsigned int)((uiScreenHeight - vPosition.y)/ uiMapTileSize_Y + startposY + 1);            
    }
    else
        endposY = MapInfo->m_uiY;

    // draw tile on screen
    vPosition.x = m_pMap->GetPositionX() + startposX * uiMapTileSize_X;
    vPosition.y = m_pMap->GetPositionY() + startposY * uiMapTileSize_Y;

    for (unsigned int i = startposY; i < endposY && startposX != endposX; ++i)
    {
        for (unsigned int j = startposX; j < endposX; ++j)
        {
            if (TileTextureSource *pTexture = m_pRessourceManager->GetMapTexture((SpriteFiles->m_sFileLocation + 
                                            SpriteFiles->m_vSpriteFiles.at(MapTiles->at(i).at(j))).c_str()))
            {
                m_pSprite->Draw(pTexture->GetTexture(), NULL, NULL, &vPosition, D3DXCOLOR(100, 100, 100, 25));
                vPosition.x += uiMapTileSize_X;
            }
        }
        vPosition.x = m_pMap->GetPositionX() + startposX * uiMapTileSize_X;
        vPosition.y += uiMapTileSize_Y;
    }
}

void MapLayer::UpdateLayer(const UINT uicCurTime, const UINT uiDiff)
{
    if (m_pMap)
    {
        D3DXVECTOR3 v3Pos = m_pMap->GetPosition();
        v3Pos.x -= 1;
        v3Pos.y -= 1;
        m_pMap->SetPosition(v3Pos);
    }
}
