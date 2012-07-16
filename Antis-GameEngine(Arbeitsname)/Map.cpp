#include "Map.h"
#include "Game.h"
#include "ResourceManager.h"
#include "MapAI.h"
#include <msxml.h>

#import <msxml4.dll>

Map::Map(void) : m_Position(0,0), m_MapLoadState(MAP_STATE_NONE), m_pMapLoadThread(NULL), m_MapColor(1,1,1,1), m_pMapAI(NULL)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Map : ";
}

Map::~Map(void)
{
    ClearMap();
}

void Map::ClearMap()
{
    if (m_pMapAI)
    {
        m_pMapAI->OnClose();
        delete m_pMapAI;
        m_pMapAI = NULL;
    }

    // clear layer
    ClearAllLayer();
    // clear mapInfo
    m_MapInfo.Clear();
    // cleare tiles
    m_v2MapTiles.clear();
    // clear ScriptPoints
    ClearScriptPoints();

    if (m_pMapLoadThread)
    {
        m_pMapLoadThread->Kill();
        m_pMapLoadThread = NULL;
    }
}

WorldObject* Map::GetObject(UINT t_uiObjGUID)
{
    std::map<UINT, WorldObject*>::iterator t_itr = m_WorldObjectLIST.find(t_uiObjGUID);
    if (t_itr == m_WorldObjectLIST.end())
        return NULL;
    
    return t_itr->second;
}

void Map::ClearScriptPoints()
{
    for (ScriptPointLIST::iterator t_itr = m_ScriptPoints.begin(); t_itr != m_ScriptPoints.end(); ++t_itr)
    {
        if (*t_itr)
            delete *t_itr;
    }
    m_ScriptPoints.clear();
}

MapLoadResult Map::LoadNewMap(UINT p_uiID)
{
    MapLoadResult result = MAP_RESULT_NONE;

    if (!m_pMapLoadThread)
    {
        if (DATABASE::Database *t_pDB = DATABASE::Database::Get())
        {
            m_MapInfo.m_uiID = p_uiID;
            m_pMapLoadThread = new MapLoadThread(t_pDB->GetMapName(p_uiID), m_MapInfo, m_v2MapTiles, m_lLayers, m_WorldObjectLIST, m_ScriptPoints);
            return MAP_RESULT_IN_PROGRESS;
        }
        else
            result = MAP_RESULT_FAILED;
    }

    if (m_pMapLoadThread && m_pMapLoadThread->GetMapLoadState() == MAP_STATE_DONE)
    {
        m_pMapLoadThread->Kill();
        m_pMapLoadThread = NULL;
        result = MAP_RESULT_DONE;
        // set owner for map layers
        for (LayerList::iterator itr = m_lLayers.begin(); itr != m_lLayers.end(); ++itr)
        {
            ObjectLayer *pLayer = (ObjectLayer*)*itr;
            if (pLayer)
                pLayer->SetOwnerMap(this);
        }

        if (m_pMapAI)
            m_pMapAI->OnLoad();
    }

    return result;
}

WorldObject* Map::AddNewWorldObject(UINT uiObjectID, int XPos, int YPos, UINT uiLayerNr)
{
    WorldObject *pNewObject = NULL;
    DATABASE::Database *t_pDB = DATABASE::Database::Get();
    if (!t_pDB)
        return NULL;

    DATABASE::ObjectPrototype const *pProto = t_pDB->GetObjectPrototype(uiObjectID);
    if (!pProto)
        return NULL;

    UINT uiGUID = GetGUIDForNewObject();
    if (!uiGUID)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to get a new GUID for Object.");
        return NULL;
    }

    switch(pProto->m_uiType)
    {
    case DATABASE::OBJECT_TYPE_MAP_OBJECT:
        pNewObject = new WorldObject(uiGUID, Point<int>(XPos, YPos));
        break;
    case DATABASE::OBJECT_TYPE_NPC:
        pNewObject = new Unit(uiGUID, Point<int>(XPos, YPos));
        break;
    default:
        break;
    }

    // insert object in a specific layer
    if (ObjectLayer *pLayer = (ObjectLayer*)GetLayerAtNr(uiLayerNr))
    {
        pNewObject->SetObjectInfo(pProto);
        if (DATABASE::SpritePrototype const *pSpriteProto = t_pDB->GetSpritePrototype(pProto->m_uiTextureID))
            pNewObject->SetTextureSource(pSpriteProto);

        pLayer->AddWorldObject(pNewObject);
        m_WorldObjectLIST.insert(std::make_pair<UINT, WorldObject*>(pNewObject->GetGUID(), pNewObject));
    }
    else
    {
        delete pNewObject;
        pNewObject = NULL;
    }

    if (m_pMapAI && pNewObject)
        m_pMapAI->OnObjectCreate(pNewObject);

    return pNewObject;
}

UINT Map::GetGUIDForNewObject()
{
    if (m_WorldObjectLIST.empty())
        return 1;

    std::map<UINT, WorldObject*>::iterator itr = --m_WorldObjectLIST.end();
    return (*itr).first + 1;
}

void Map::DrawMap()
{
    CGame *pGame = CGame::Get();
    if (!pGame)
        return;

    GameInfo *pGameInfo = pGame->GetGameInfo();
    if (!pGameInfo)
        return;

    TextureMgr *pTextureMgr = TextureMgr::Get();
    if (!pTextureMgr)
        return;

    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    // get screen resolution
    Point<UINT> windowSize                  = pGameInfo->GetWindowSize();
    const MapInfo *MapInfo                  = GetMapInfo();
    const std::vector<MapTiles> *MapTiles   = GetMapTiles();
    // return if one of the info files are corrupt
    if (!MapInfo || !MapTiles)
        return;
    
    Point<int> MapPosition = GetPosition();

    // get maptile size
    Point<UINT> tileSize = pGameInfo->GetMapTileSize();

    // get start pos left and up
    UINT startposX = 0;
    UINT startposY = 0;
    if (MapPosition.x < 0)
        startposX = (UINT)((-1) * MapPosition.x / tileSize.x);
    if (MapPosition.y < 0)
        startposY = (UINT)((-1) * MapPosition.y / tileSize.y);

    // if startpos greater max map tiles, set to max map tiles
    if (startposX > MapInfo->m_uiX)
        startposX = MapInfo->m_uiX;
    if (startposY > MapInfo->m_uiY)
        startposY = MapInfo->m_uiY;

    // get end pos right and bottom
    UINT endposX = 0;
    UINT endposY = 0;
    if (MapPosition.x + MapInfo->m_uiX * tileSize.x > windowSize.x)
    {
        endposX = startposX + ((windowSize.x+tileSize.x/2)/tileSize.x);

        if ((UINT)MapPosition.x % tileSize.x)
        {
            if (startposX > 0)
                startposX--;

            endposX++;
        }
    }
    else
        endposX = MapInfo->m_uiX;

    if (MapPosition.y + MapInfo->m_uiY * tileSize.y > windowSize.y)
    {
        endposY = startposY + ((windowSize.y+tileSize.y/2)/tileSize.y);

        if ((UINT)MapPosition.y % tileSize.y)
        {
            if (startposY > 0)
                startposY--;

            endposY++;
        }          
    }
    else
        endposY = MapInfo->m_uiY;

    // draw tile on screen
    LPD3DXSPRITE pSprite = pDirect3D->GetSpriteForDraw();
    if (!pSprite)
        return;

    // iterate through layers
    for (UINT layer = 0; layer < MapTiles->size(); layer++)
    {
        MapPosition.x = GetPositionX() + startposX * tileSize.x;
        MapPosition.y = GetPositionY() + startposY * tileSize.y;

        for (UINT i = startposY; i < endposY && startposX != endposX; ++i)
        {
            // break if empty or out of range
            if (MapTiles->at(layer).empty() || i >= MapTiles->at(layer).size())
                break;

            for (UINT j = startposX; j < endposX; ++j)
            {
                // break if empty or out of range
                if (MapTiles->at(layer).at(i).empty() || j >= MapTiles->at(layer).at(i).size())
                    break;

                float fMapTile = MapTiles->at(layer).at(i).at(j);

                // if fMapTile is a float value (e.g. x.4) its an autotile
                if ((fMapTile / (int)fMapTile) > 1)
                {
                    if (const TextureSource *pTexture = pTextureMgr->GetTextureSource((UINT)fMapTile))
                    {
                        if (pTexture->m_pTexture)
                        {
                            // set source rect
                            int iAutoTile = (int)((fMapTile +0.00005) * 10000)%10000;
                            RECT rSrcRect;
                            // if autotile is one tile
                            if (iAutoTile % 1000 == 0)
                            {
                                iAutoTile /= 10;
                                rSrcRect.left = ((iAutoTile/100-1) % 3) *tileSize.x;
                                rSrcRect.right = rSrcRect.left + tileSize.x;
                                rSrcRect.top = ((iAutoTile/100-1) / 3) *tileSize.y;
                                rSrcRect.bottom = rSrcRect.top + tileSize.y;
                                pSprite->Draw(pTexture->m_pTexture, &rSrcRect, NULL, &D3DXVECTOR3((float)MapPosition.x, (float)MapPosition.y, 0), GetColor());
                            }
                            // if autotile is split up into 4 tiles
                            else if (iAutoTile % 10 > 0)
                            {
                                D3DXVECTOR3 vPosTemp;
                                UINT iCount = 1;
                                for (int i = 0; i < 2; i++)
                                {
                                    for (int j = 0; j < 2; j++)
                                    {
                                        // check x pos
                                        rSrcRect.left = iAutoTile%((int)pow(10.0f, (float)iCount));
                                        rSrcRect.left /= (int)pow(10.0f, (float)iCount-1);
                                        rSrcRect.left--;
                                        rSrcRect.left = rSrcRect.left % 3 *(tileSize.x / (j+1));
                                        if (!j)
                                            rSrcRect.left += tileSize.x / 2;

                                        rSrcRect.right = rSrcRect.left + tileSize.x / 2;


                                        // check y pos
                                        rSrcRect.top = iAutoTile%((int)pow(10.0f, (float)iCount));
                                        rSrcRect.top /= (int)pow(10.0f, (float)iCount-1);
                                        rSrcRect.top--;
                                        rSrcRect.top = rSrcRect.top / 3 *(tileSize.y / (i+1));
                                        if (!i)
                                            rSrcRect.top += tileSize.y / 2;

                                        rSrcRect.bottom = rSrcRect.top + tileSize.y / 2;

                                        // calc draw pos
                                        if (j)
                                            vPosTemp.x = (float)MapPosition.x;
                                        else
                                            vPosTemp.x = (float)MapPosition.x + tileSize.x / 2;

                                        if (i)
                                            vPosTemp.y = (float)MapPosition.y;
                                        else
                                            vPosTemp.y = (float)MapPosition.y + tileSize.y / 2;
                                        vPosTemp.z = 0;

                                        pSprite->Draw(pTexture->m_pTexture, &rSrcRect, NULL, &vPosTemp, GetColor());
                                        iCount++;
                                    }
                                }
                            }
                            // if autotile is split up into 2 tiles
                            else
                            {
                                D3DXVECTOR3 vPosTemp((float)MapPosition.x, (float)MapPosition.y, 0);
                                iAutoTile /= 10;
                                for (int i = 0; i < 2; i++)
                                {
                                    // if autotile is left to right split up
                                    if (iAutoTile % 10 == 0)
                                    {
                                        if (i == 0)
                                            rSrcRect.left = ((iAutoTile/100-1) % 3) *tileSize.x;
                                        else
                                            rSrcRect.left = ((iAutoTile/10%10-1) % 3) *tileSize.x;
                                        rSrcRect.right = rSrcRect.left + tileSize.x;

                                        if (i == 0)
                                            rSrcRect.top = ((iAutoTile/100-1) / 3) *tileSize.y;
                                        else
                                            rSrcRect.top = ((iAutoTile/10%10-1) / 3) *tileSize.y;

                                        if (i == 1)
                                            rSrcRect.top += tileSize.y/2;
                                        rSrcRect.bottom = rSrcRect.top + tileSize.y/2;
                                    }
                                    // if autotile is top to bottom split up
                                    else
                                    {
                                        if (i == 0)
                                            rSrcRect.left = ((iAutoTile/100-1) % 3) *tileSize.x;
                                        else
                                            rSrcRect.left = ((iAutoTile/10%10-1) % 3) *tileSize.x;

                                        if (i == 1)
                                            rSrcRect.left +=tileSize.x/2;
                                        rSrcRect.right = rSrcRect.left + tileSize.x/2;

                                        if (i == 0)
                                            rSrcRect.top = ((iAutoTile/100-1) / 3) * tileSize.y;
                                        else
                                            rSrcRect.top = ((iAutoTile/10%10-1) / 3) * tileSize.y;

                                        rSrcRect.bottom = rSrcRect.top + tileSize.y;
                                    }
                                    pSprite->Draw(pTexture->m_pTexture, &rSrcRect, NULL, &vPosTemp, GetColor());
                                    if (iAutoTile % 10 == 0)
                                        vPosTemp.y += tileSize.y/2;
                                    else
                                        vPosTemp.x += tileSize.x/2;
                                }
                            }
                        }
                    }
                }
                else    // normal map tile
                {
                    if (const TextureSource *pTexture = pTextureMgr->GetTextureSource((UINT)fMapTile))
                    {
                        if (pTexture->m_pTexture)   
                            pSprite->Draw(pTexture->m_pTexture, NULL, NULL, &D3DXVECTOR3((float)MapPosition.x, (float)MapPosition.y, 0), GetColor());
                    }
                }
                MapPosition.x += tileSize.x;
            }
            MapPosition.x = GetPositionX() + startposX * tileSize.x;
            MapPosition.y += tileSize.y;
        }
    }

    pDirect3D->EndSpriteDraw();
}

void Map::DrawLayer()
{
    // iterate through layerlist
    for (LayerList::iterator itr = m_lLayers.begin(); itr != m_lLayers.end(); ++itr)
    {
        if (*itr)
            (*itr)->DrawLayer();
    }
}

void Map::Draw()
{
    DrawMap();
    DrawLayer();
}

void Map::AddLayer(Layer *pLayer)
{
    if (!pLayer)
        return;

    m_lLayers.push_back(pLayer);
}

void Map::EraseLayer(UINT uiLayerNr)
{
    if (m_lLayers.empty() || uiLayerNr > m_lLayers.size() -1)
        return;

    Layer *pLayer = m_lLayers.at(uiLayerNr);
    EraseLayer(pLayer);
}

void Map::EraseLayer(Layer *pLayer)
{
    if (!pLayer)
        return;

    // iterate through layerlist
    for (LayerList::iterator itr = m_lLayers.begin(); itr != m_lLayers.end(); ++itr)
    {
        if ((*itr) && (*itr) == pLayer)
        {
            delete *itr;
            m_lLayers.erase(itr);
            break;
        }
    }
}

void Map::ClearAllLayer()
{
    // iterate through layerlist
    for (LayerList::iterator itr = m_lLayers.begin(); itr != m_lLayers.end(); ++itr)
    {
        if ((*itr))
            delete *itr;
    }
    m_lLayers.clear();
    m_WorldObjectLIST.clear();
}

Layer* Map::GetLayerAtNr(UINT uiLayerNr)
{
    if (m_lLayers.empty() || uiLayerNr > m_lLayers.size() -1)
        return NULL;

    return m_lLayers.at(uiLayerNr);
}

void Map::UpdateMap(const ULONGLONG uiCurTime, const UINT uiDiff)
{
    // iterate through layerlist
    for (LayerList::iterator itr = m_lLayers.begin(); itr != m_lLayers.end(); ++itr)
    {
        if ((*itr))
            (*itr)->UpdateLayer(uiCurTime, uiDiff);
    }
}

bool Map::IsPassable(UINT XPos, UINT YPos, PassabilityFlag MoveDirection)
{
    if (m_MapInfo.m_uiX <= XPos || m_MapInfo.m_uiY <= YPos)
        return false;

    TextureMgr *pTextureMgr = TextureMgr::Get();
    if (!pTextureMgr)
        return false;

    // iterate through layers
    for (UINT i = 0; i < m_v2MapTiles.size(); i++)
    {
        if (const TextureSource *pTexture = pTextureMgr->GetTextureSource((UINT)m_v2MapTiles.at(i).at(YPos).at(XPos)))
        {
            switch(pTexture->m_TextureInfo.m_uiSpriteType)
            {
            case 0:
            case 1:
                if (!(pTexture->m_TextureInfo.Type.Tile.m_uiPassable & MoveDirection))
                    return false;
                break;
            default:
                break;
            }
        }
    }

    return true;
}

Point<UINT> Map::CalcPixToTile(Point<UINT> uiPos)
{
    Point<UINT> uiTileSize;
    if (CGame *pGame = CGame::Get())
    {
        if (GameInfo *pInfo = pGame->GetGameInfo())
            uiTileSize = pInfo->GetMapTileSize();
    }

    Point<UINT> uiTile = uiPos;
    if (uiTileSize.x != 0)
        uiTile.x = uiPos.x / uiTileSize.x;

    if (uiTileSize.y != 0)
        uiTile.y = uiPos.y / uiTileSize.y;

    return uiTile;
}

/*#####
## MapLoadThread
#####*/
MapLoadThread::MapLoadThread(std::string sMapName, MapInfo &MapInfo, std::vector<MapTiles> &MapTiles, LayerList &LayerList, std::map<UINT, WorldObject*> &objectList, ScriptPointLIST &scriptPoints)
    : m_MapInfo(MapInfo), m_v2MapTiles(MapTiles), m_lLayers(LayerList), m_ScriptPoints(scriptPoints), m_WorldObjectLIST(objectList), ActiveObject()
{
    m_MapLoadState      = MAP_STATE_NONE;
    m_sMapName          = sMapName;
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "MapLoadThread : ";

    _thread.Resume ();
}

void MapLoadThread::Run()
{
    if (FAILED(CoInitialize(NULL)))
        ERROR_LOG(m_sLogLocationName + "Unable to initialize COM.");

    MapLoadResult result = MAP_RESULT_NONE;
    std::string sMapDataFromFile;

    while(m_MapLoadState != MAP_STATE_DONE)
    {
        switch(m_MapLoadState)
        {
        case MAP_STATE_NONE:                        // Load data from file
            LoadDataFromFile(m_sMapName, &sMapDataFromFile);
            m_MapLoadState = MAP_STATE_DO_INFO;
            break;
        case MAP_STATE_DO_INFO:                     // Load map info
            result = LoadInfo(&sMapDataFromFile);
            m_MapLoadState = MAP_STATE_DO_TILES;
            break;
        case MAP_STATE_DO_TILES:                    // Load tiles
            result = LoadTiles(&sMapDataFromFile);
            m_MapLoadState = MAP_STATE_DO_OBJECTS;
            break;
        case MAP_STATE_DO_OBJECTS:                  // Load objects and layer
            result = LoadLayerAndObjects(&sMapDataFromFile);
            m_MapLoadState = MAP_STATE_DO_SCRIPT_POINTS;
            break;
        case MAP_STATE_DO_SCRIPT_POINTS:            // Load script points
            result = LoadScriptPoints(&sMapDataFromFile);
            m_MapLoadState = MAP_STATE_DONE;
            break;
        default:
            break;
        }

        switch(result)
        {
        case MAP_RESULT_CORRUPT_FILE:                   // if map file has syntax mistake or info missing
            ERROR_LOG(m_sLogLocationName + m_sMapName + " is a corrupt file. Check syntax or rebuild it.");
            break;
        case MAP_RESULT_NO_FILE:                        // if unable to find file
            ERROR_LOG(m_sLogLocationName + "Unable to find " + m_sMapName + ". Check filename or directory.");
            break;
        case MAP_RESULT_FAILED:
            ERROR_LOG(m_sLogLocationName + m_sMapName + " causes an undefined error. Check syntax or rebuild it.");
            break;
        case MAP_RESULT_OK:                             // if result = OK and MapState = Done - write into log
            if (m_MapLoadState == MAP_STATE_DONE)
                BASIC_LOG(m_sLogLocationName + "Successfully load " + m_sMapName + ".");
            break;
        default:
            break;
        }
    }

    CoUninitialize();
}

MapLoadResult MapLoadThread::LoadDataFromFile(std::string sMapName, std::string *sMapData)
{
    // this open and store the XML file:
    std::ifstream Data(("Maps/" + sMapName).c_str());
    if(!Data)
        return MAP_RESULT_NO_FILE;

    std::string DataLine;
    while(std::getline(Data,DataLine))
        *sMapData += DataLine;

    return MAP_RESULT_OK;
}

MapLoadResult MapLoadThread::LoadInfo(std::string *sMapData)
{
    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument40));
    if (hr != S_OK)
        return MAP_RESULT_FAILED;

    if (!sMapData->empty())
        pXMLDom->loadXML(sMapData->c_str());

    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("MapInfo");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    LONG iAttributLength = 0;
    IXMLDOMNamedNodeMapPtr pmAttributes;
    hr = pNode->get_attributes(&pmAttributes);
    if (hr != S_OK)
        return MAP_RESULT_CORRUPT_FILE;

    pmAttributes->get_length(&iAttributLength);
    VARIANT value;
    VariantInit(&value);
    for (int i=0; i  < iAttributLength; i++)
    {
        pmAttributes->get_item(i, &pNode);
        if (!pNode)
            continue;

        hr = pNode->get_nodeValue(&value);
        if (hr == S_OK)
        {
            switch(i)
            {
            case 0:                 // map name
                m_MapInfo.m_sMapName = _bstr_t(value.bstrVal);
                break;
            case 1:                 // announce name
                m_MapInfo.m_sMapAnnounceName = _bstr_t(value.bstrVal);
                break;
            case 2:                 // store map size
                m_MapInfo.m_uiX = atoi(_bstr_t(value.bstrVal));
                break;
            case 3:
                m_MapInfo.m_uiY = atoi(_bstr_t(value.bstrVal));
                break;
            default:
                break;
            }
        }
    }

    return MAP_RESULT_OK;
}

MapLoadResult MapLoadThread::LoadTiles(std::string *sMapData)
{
    std::vector<std::list<std::string>> TileByRowFromFileList;

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument40));
    if (hr != S_OK)
        return MAP_RESULT_FAILED;

    if (!sMapData->empty())
        pXMLDom->loadXML(sMapData->c_str());

    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    // get tile map from file
    pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("MapData");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    IXMLDOMNodeListPtr pLayerNodes;
    hr = pNode->get_childNodes(&pLayerNodes);
    if (hr != S_OK)
        return MAP_RESULT_CORRUPT_FILE;

    // iterate through layers
    LONG iLayerLength = 0;
    pLayerNodes->get_length(&iLayerLength);
    IXMLDOMNodePtr pLayer = NULL;
    for (LONG j = 0; j < iLayerLength; j++)
    {
        hr = pLayerNodes->get_item(j, &pLayer);
        if (hr != S_OK)
            continue;

        // get tiles
        IXMLDOMNodeListPtr pMapNodes;
        hr = pLayer->get_childNodes(&pMapNodes);
        if (hr != S_OK)
            continue;

        LONG iNodeLength = 0;
        pMapNodes->get_length(&iNodeLength);
        IXMLDOMNamedNodeMapPtr pmAttributes;

        BSTR attributeText = NULL;

        // iterate through tiles
        IXMLDOMNodePtr pNodeTemp = NULL;
        std::list<std::string> tempList;
        for (int i = 0; i < iNodeLength; i++)
        {
            // continue if there are no "Tiles" set
            hr = pMapNodes->get_item(i, &pNodeTemp);
            if (hr == S_FALSE)
                continue;

            hr = pNodeTemp->get_attributes(&pmAttributes);
            if (hr == S_FALSE)
                continue;

            hr = pmAttributes->get_item(0, &pNodeTemp);
            if (hr == S_FALSE)
                continue;

            hr = pNodeTemp->get_text(&attributeText);
            if (hr == S_FALSE)
                continue;

            std::string temp = _bstr_t(attributeText);
            tempList.push_back(temp);
        }
        TileByRowFromFileList.push_back(tempList);
    }

    // read out layers
    for (UINT i = 0; i < TileByRowFromFileList.size(); i++)
    {
        MapTiles tempTiles;
        // if tiles are in string, read it out and store as readable vector.
        while(!TileByRowFromFileList.at(i).empty())
        {
            std::vector<float> vTempTiles;
            std::list<std::string>::iterator itr = TileByRowFromFileList.at(i).begin();
            // read out rows
            while(!(*itr).empty())
            {
                // if first sign is a number
                if ((*itr).at(0) <= 57 && (*itr).at(0) >= 48)
                {
                    vTempTiles.push_back((float)atof((*itr).c_str()));
                    (*itr).erase(0, (*itr).find(32));
                }
                // if first sign is a not a number, delete it
                else 
                    (*itr).erase(0, 1);
            }
            tempTiles.push_back(vTempTiles);
            TileByRowFromFileList.at(i).erase(itr);
        }
        m_v2MapTiles.push_back(tempTiles);
    }

    return MAP_RESULT_OK;
}

MapLoadResult MapLoadThread::LoadLayerAndObjects(std::string *sMapData)
{
    std::vector<std::vector<ObjectReadOut*>> vLayerAndObjects;

    std::list<std::string> TileByRowFromFileList;

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument40));
    if (hr != S_OK)
        return MAP_RESULT_FAILED;

    if (!sMapData->empty())
        pXMLDom->loadXML(sMapData->c_str());

    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    // get layer + objects from file
    std::vector<std::vector<WorldObject*>> lLayerData;
    pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("LayerData");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    // read out layers
    IXMLDOMNodeListPtr pLayerNodeList;
    hr = pNode->get_childNodes(&pLayerNodeList);
    if (hr != S_OK)
        return MAP_RESULT_CORRUPT_FILE;

    // iterate through layers
    LONG iLayerNodeLength = 0;
    pLayerNodeList->get_length(&iLayerNodeLength);
    IXMLDOMNodePtr pLayerNode = NULL;
    IXMLDOMNodeListPtr pObjectNodeList;
    for (LONG i = 0; i < iLayerNodeLength; i++)
    {
        // select layer
        hr = pLayerNodeList->get_item(i, &pLayerNode);
        if (hr == S_FALSE)
            continue;

        // read out objects of layer
        pObjectNodeList.Detach();
        hr = pLayerNode->get_childNodes(&pObjectNodeList);
        if (hr == S_FALSE)
            continue;

        // iterate through objects
        LONG iObjectNodeLength = 0;
        pObjectNodeList->get_length(&iObjectNodeLength);
        IXMLDOMNodePtr pObjectNode = NULL;
        std::vector<ObjectReadOut*> vObjects;
        IXMLDOMNamedNodeMapPtr pmObjectAttributes;
        for (LONG j = 0; j < iObjectNodeLength; j++)
        {
            // select object
            hr = pObjectNodeList->get_item(j, &pObjectNode);
            if (hr == S_FALSE)
                continue;

            // read out object attributes
            pmObjectAttributes.Detach();
            hr = pObjectNode->get_attributes(&pmObjectAttributes);
            if (hr == S_FALSE)
                continue;

            // iterate through attributes
            LONG iAttributeLength = 0;
            pmObjectAttributes->get_length(&iAttributeLength);
            IXMLDOMNodePtr pSelectedAttribute = NULL;
            VARIANT value;
            VariantInit(&value);
            BSTR sNodeName;
            ObjectReadOut *newObject = new ObjectReadOut();
            for (LONG k = 0; k < iAttributeLength; k++)
            {
                // select layer
                hr = pmObjectAttributes->get_item(k, &pSelectedAttribute);
                if (hr == S_FALSE)
                    continue;

                // get node value
                hr = pSelectedAttribute->get_nodeValue(&value);
                if (hr == S_FALSE)
                    continue;

                // get node name
                hr = pSelectedAttribute->get_nodeName(&sNodeName);
                if (hr == S_FALSE)
                    continue;

                std::string sNode = _bstr_t(sNodeName);
                if (sNode == "ID")
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    newObject->m_ObjectID = value.uintVal;
                }
                else if (sNode == "GUID")
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    newObject->m_GUID = value.uintVal;
                }
                else if (sNode == "XPos")
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    newObject->m_XPos = value.uintVal;
                }
                else if (sNode == "YPos")
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    newObject->m_YPos = value.uintVal;
                }
                else if (sNode == "Direction")
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    newObject->m_uiDirection = value.uintVal;
                }
                else if (sNode == "Walkmode")
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    newObject->m_uiWalkmode = value.uintVal;
                }
            }
            vObjects.push_back(newObject);
        }
        vLayerAndObjects.push_back(vObjects);
    }

    // iterate through vector and create layer + objects
    if (!vLayerAndObjects.empty())
    {
        for (UINT i = 0; i < vLayerAndObjects.size(); i++)
        {
            if (vLayerAndObjects.at(i).empty())
                continue;
            
            ObjectLayer *pLayer = new ObjectLayer();
            for (UINT j = 0; j < vLayerAndObjects.at(i).size(); j++)
            {
                if (!vLayerAndObjects.at(i).at(j))
                    continue;

                // continue if there is the same guid set as we need now
                std::map<UINT, WorldObject*>::iterator itr = m_WorldObjectLIST.find(vLayerAndObjects.at(i).at(j)->m_GUID);
                if (itr != m_WorldObjectLIST.end())
                    continue;

                WorldObject *pObject = NULL;
                if (DATABASE::Database *t_pDB = DATABASE::Database::Get())
                {
                    if (const DATABASE::ObjectPrototype *pProto = t_pDB->GetObjectPrototype(vLayerAndObjects.at(i).at(j)->m_ObjectID))
                    {
                        // set new object for specific object type
                        switch(pProto->m_uiType)
                        {
                        case DATABASE::OBJECT_TYPE_NPC:
                            pObject = new Unit(vLayerAndObjects.at(i).at(j)->m_GUID, Point<int>(vLayerAndObjects.at(i).at(j)->m_XPos, vLayerAndObjects.at(i).at(j)->m_YPos),
                                WrapToDirection(vLayerAndObjects.at(i).at(j)->m_uiDirection), WrapToWalkmode(vLayerAndObjects.at(i).at(j)->m_uiWalkmode));
                            break;
                        case DATABASE::OBJECT_TYPE_MAP_OBJECT:
                        default:
                            pObject = new WorldObject(vLayerAndObjects.at(i).at(j)->m_GUID, Point<int>(vLayerAndObjects.at(i).at(j)->m_XPos, vLayerAndObjects.at(i).at(j)->m_YPos));
                            break;
                        }
                        pObject->SetObjectInfo(pProto);
                    }

                    if (pObject)
                    {
                        pObject->SetTextureSource(t_pDB->GetSpritePrototype(pObject->GetObjectInfo()->m_uiTextureID));

                        m_WorldObjectLIST.insert(std::make_pair<UINT, WorldObject*>(vLayerAndObjects.at(i).at(j)->m_GUID, pObject));
                        pLayer->AddWorldObject(pObject);
                    }
                }

                delete vLayerAndObjects.at(i).at(j);
            }

            m_lLayers.push_back(pLayer);
        }
    }

    return MAP_RESULT_OK;
}

MapLoadResult MapLoadThread::LoadScriptPoints(std::string *sMapData)
{
    m_ScriptPoints.clear();

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument40));
    if (hr != S_OK)
        return MAP_RESULT_FAILED;

    if (!sMapData->empty())
        pXMLDom->loadXML(sMapData->c_str());

    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    // get layer + objects from file
    std::vector<std::vector<WorldObject*>> lLayerData;
    pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("ScriptPoints");
    if (!pNode)
        return MAP_RESULT_CORRUPT_FILE;

    IXMLDOMNodeListPtr pObjectNodeList;

    // read script points
    pObjectNodeList.Detach();
    hr = pNode->get_childNodes(&pObjectNodeList);
    if (hr == S_FALSE)
        return MAP_RESULT_CORRUPT_FILE;

    // iterate through objects
    LONG iObjectNodeLength = 0;
    pObjectNodeList->get_length(&iObjectNodeLength);
    IXMLDOMNodePtr pObjectNode = NULL;
    IXMLDOMNamedNodeMapPtr pmObjectAttributes;
    for (LONG j = 0; j < iObjectNodeLength; j++)
    {
        // select object
        hr = pObjectNodeList->get_item(j, &pObjectNode);
        if (hr == S_FALSE)
            continue;

        // read out object attributes
        pmObjectAttributes.Detach();
        hr = pObjectNode->get_attributes(&pmObjectAttributes);
        if (hr == S_FALSE)
            continue;

        // iterate through attributes
        LONG iAttributeLength = 0;
        pmObjectAttributes->get_length(&iAttributeLength);
        IXMLDOMNodePtr pSelectedAttribute = NULL;
        VARIANT value;
        VariantInit(&value);
        BSTR sNodeName;
        ScriptPointPrototype newPoint;
        for (LONG k = 0; k < iAttributeLength; k++)
        {
            // select layer
            hr = pmObjectAttributes->get_item(k, &pSelectedAttribute);
            if (hr == S_FALSE)
                continue;

            // get node value
            hr = pSelectedAttribute->get_nodeValue(&value);
            if (hr == S_FALSE)
                continue;

            // get node name
            hr = pSelectedAttribute->get_nodeName(&sNodeName);
            if (hr == S_FALSE)
                continue;

            std::string sNode = _bstr_t(sNodeName);

            if (sNode == "XPos")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                newPoint.m_Position.x = value.uintVal;
            }
            else if (sNode == "YPos")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                newPoint.m_Position.y = value.uintVal;
            }
            else if (sNode == "XSize")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                newPoint.m_Size.x = value.uintVal;
            }
            else if (sNode == "YSize")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                newPoint.m_Size.y = value.uintVal;
            }
            else if (sNode == "Type")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                newPoint.m_Type = WrapToScriptPointType(value.uintVal);
            }
            else if (sNode == "MapID")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                switch(newPoint.m_Type)
                {
                case SCRIPT_POINT_TELEPORT:
                    newPoint.Type.Teleport.m_uiMapID = value.uintVal;
                    break;
                default:
                    break;
                }
            }
            else if (sNode == "PosX")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                switch(newPoint.m_Type)
                {
                case SCRIPT_POINT_TELEPORT:
                    newPoint.Type.Teleport.m_uiTelePosX = value.uintVal;
                    break;
                default:
                    break;
                }
            }
            else if (sNode == "PosY")
            {
                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                switch(newPoint.m_Type)
                {
                case SCRIPT_POINT_TELEPORT:
                    newPoint.Type.Teleport.m_uiTelePosY = value.uintVal;
                    break;
                default:
                    break;
                }
            }
        }
        ScriptPoint *pPoint = new ScriptPoint();
        pPoint->SetScriptPointInfo(&newPoint);
        m_ScriptPoints.push_back(pPoint);
    }
    return MAP_RESULT_OK;
}