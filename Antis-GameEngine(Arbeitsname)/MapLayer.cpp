#include "MapLayer.h"
#include "Game.h"
#include <cmath>

MapLayer::MapLayer(void) : Layer()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "MapLayer : ";
    m_LayerType         = LAYER_TYPE_MAP;
    m_pMap              = new Map();
}

MapLayer::~MapLayer(void)
{
    if (m_pMap)
        delete m_pMap;
}

MapLoadResult MapLayer::LoadNewMap(std::string sMapName)
{
    if (!m_pMap)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to find a valid pointer to map. Return without loading new map.");
        return MAP_RESULT_FAILED;
    }

    return m_pMap->LoadNewMap(sMapName);
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
    SpriteFiles *SpriteFiles        = m_pMap->GetSpriteFiles();
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

    // if startpos greater max map tiles, set to max map tiles
    if (startposX > MapInfo->m_uiX)
        startposX = MapInfo->m_uiX;
    if (startposY > MapInfo->m_uiY)
        startposY = MapInfo->m_uiY;

    // get end pos right and bottom
    unsigned int endposX = 0;
    unsigned int endposY = 0;
    if (vPosition.x + MapInfo->m_uiX * uiMapTileSize_X > uiScreenWidth)
    {
        if (vPosition.x < 0)
            endposX = (unsigned int)(-1*(vPosition.x + startposX * uiMapTileSize_X) + uiScreenWidth ) / uiMapTileSize_X + startposX + 1;

        else if (vPosition.x < uiScreenWidth)
            endposX = (unsigned int)((uiScreenWidth - vPosition.x)/ uiMapTileSize_X + startposX + 1);
    }
    else
        endposX = MapInfo->m_uiX;

    if (vPosition.y + MapInfo->m_uiY * uiMapTileSize_Y > uiScreenHeight)
    {
        if (vPosition.y < 0)
            endposY = (unsigned int)(-1*(vPosition.y + startposY * uiMapTileSize_X) + uiScreenHeight ) / uiMapTileSize_Y + startposY + 1;
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
        // break if empty or out of range
        if (MapTiles->empty() || MapTiles->size()-1 < i)
            break;

        for (unsigned int j = startposX; j < endposX; ++j)
        {
            // break if empty or out of range
            if (MapTiles->at(i).empty() || MapTiles->at(i).size()-1 < j)
                break;

            float fMapTile = MapTiles->at(i).at(j);

            // if fMapTile is a float value (e.g. x.4) its an autotile
            if ((fMapTile / (int)fMapTile) > 1)
            {
                if (TileTextureSource *pTexture = m_pRessourceManager->GetMapTexture(SpriteFiles->GetSpriteFileAt(SPRITE_TYPE_AUTOTILE, (UINT)fMapTile)))
                {
                    if (pTexture->GetTexture())
                    {
                        // set source rect
                        float temp = fMapTile - floor(fMapTile);
                        temp *= 10;
                        int iAutoTile = (int)(temp+0.5f);
                        RECT rSrcRect;
                        rSrcRect.left = ((iAutoTile-1) % 3) *uiMapTileSize_X;
                        rSrcRect.right = rSrcRect.left + uiMapTileSize_X;
                        rSrcRect.top = ((iAutoTile-1) / 3) *uiMapTileSize_Y;
                        rSrcRect.bottom = rSrcRect.top + uiMapTileSize_Y;
                        m_pSprite->Draw(pTexture->GetTexture(), &rSrcRect, NULL, &vPosition, D3DXCOLOR(100, 100, 100, 25));
                    }
                }
            }
            else    // normal map tile
            {
                if (TileTextureSource *pTexture = m_pRessourceManager->GetMapTexture(SpriteFiles->GetSpriteFileAt(SPRITE_TYPE_MAP, (UINT)fMapTile)))
                {
                    if (pTexture->GetTexture())   
                            m_pSprite->Draw(pTexture->GetTexture(), NULL, NULL, &vPosition, D3DXCOLOR(100, 100, 100, 25));
                }
            }
            vPosition.x += uiMapTileSize_X;
        }
        vPosition.x = m_pMap->GetPositionX() + startposX * uiMapTileSize_X;
        vPosition.y += uiMapTileSize_Y;
    }
}

void MapLayer::UpdateLayer(const UINT uicCurTime, const UINT uiDiff)
{
    /*if (m_pMap)
    {
        D3DXVECTOR3 v3Pos = m_pMap->GetPosition();
        v3Pos.x -= 1;
        v3Pos.y -= 2;
        m_pMap->SetPosition(v3Pos);
    }*/
}
