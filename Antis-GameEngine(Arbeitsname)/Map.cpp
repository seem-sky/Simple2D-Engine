#include "Map.h"
#include <msxml2.h>
#include "Time.h"
#include "Game.h"
#include "RessourceManager.h"
#include "Direct3D.h"

#import <msxml4.dll>

Map::Map(void)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Map : ";

    m_pSpriteFiles      = SpriteFiles::Get();

    m_v3Position.x = 0;
    m_v3Position.y = 0;
    m_v3Position.z = 0;

    m_MapLoadState = MAP_STATE_NONE;
    m_pMapLoadThread = NULL;

    m_MapColor.r = 1;
    m_MapColor.g = 1;
    m_MapColor.b = 1;
    m_MapColor.a = 1;
}

Map::~Map(void)
{
    if (m_pMapLoadThread)
        m_pMapLoadThread->Kill();

    // clear all layers
    ClearAllLayer();
}

MapLoadResult Map::LoadNewMap(std::string sMapName)
{
    MapLoadResult result = MAP_RESULT_NONE;

    if (!m_pMapLoadThread)
        m_pMapLoadThread = new MapLoadThread(sMapName);

    if (m_pMapLoadThread && m_pMapLoadThread->GetMapLoadState() == MAP_STATE_DONE)
    {
        m_pMapLoadThread->GetMapInfo(m_MapInfo, m_v2MapTiles, m_lLayers);
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
    }

    return result;
}

WorldObject* Map::AddNewWorldObject(std::string sFileName, int XPos, int YPos, UINT uiLayerNr)
{
    WorldObject *pNewObject = new WorldObject();
    // insert object in a specific layer
    if (ObjectLayer *pLayer = (ObjectLayer*)GetLayerAtNr(uiLayerNr))
    {
        pNewObject->SetPosition(D3DXVECTOR2((float)XPos, (float)YPos));
        pNewObject->SetTextureSource(sFileName);
        pLayer->AddWorldObject(pNewObject);
    }
    else
    {
        delete pNewObject;
        pNewObject = NULL;
    }

    return pNewObject;
}

void Map::DrawMap()
{
    CGame *pGame = CGame::Get();
    if (!pGame)
        return;

    CGameInfo *pGameInfo = pGame->GetGameInfo();
    if (!pGameInfo)
        return;

    CRessourceManager *pRManager = CRessourceManager::Get();
    if (!pRManager)
        return;

    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    // get screen resolution
    unsigned int uiScreenWidth = 0;
    unsigned int uiScreenHeight = 0;
    pGameInfo->GetWindowSize(uiScreenWidth, uiScreenHeight);

    const MapInfo *MapInfo          = GetMapInfo();
    const MapTiles *MapTiles        = GetMapTiles();
    SpriteFiles *SpriteFiles        = GetSpriteFiles();
    // return if one of the info files are corrupt
    if (!MapInfo || !MapTiles || !SpriteFiles)
        return;
    
    D3DXVECTOR3 vPosition = GetPosition();

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
    LPD3DXSPRITE pSprite = pDirect3D->GetSpriteForDraw();
    if (!pSprite)
        return;

    vPosition.x = GetPositionX() + startposX * uiMapTileSize_X;
    vPosition.y = GetPositionY() + startposY * uiMapTileSize_Y;

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
                if (TileTextureSource *pTexture = pRManager->GetMapTexture(SpriteFiles->GetSpriteFileAt(SPRITE_TYPE_AUTOTILE, (UINT)fMapTile)))
                {
                    if (pTexture->GetTexture())
                    {
                        // set source rect
                        int iAutoTile = (int)((fMapTile +0.00005) * 10000)%10000;
                        RECT rSrcRect;
                        // if autotile is one tile
                        if (iAutoTile % 1000 == 0)
                        {
                            iAutoTile /= 10;
                            rSrcRect.left = ((iAutoTile/100-1) % 3) *uiMapTileSize_X;
                            rSrcRect.right = rSrcRect.left + uiMapTileSize_X;
                            rSrcRect.top = ((iAutoTile/100-1) / 3) *uiMapTileSize_Y;
                            rSrcRect.bottom = rSrcRect.top + uiMapTileSize_Y;
                            pSprite->Draw(pTexture->GetTexture(), &rSrcRect, NULL, &vPosition, GetColor());
                        }
                        // if autotile is split up into 4 tiles
                        else if (iAutoTile % 10 > 0)
                        {
                            D3DXVECTOR3 vPosTemp;
                            unsigned int iCount = 1;
                            for (int i = 0; i < 2; i++)
                            {
                                for (int j = 0; j < 2; j++)
                                {
                                    // check x pos
                                    rSrcRect.left = iAutoTile%((int)pow(10.0f, (float)iCount));
                                    rSrcRect.left /= (int)pow(10.0f, (float)iCount-1);
                                    rSrcRect.left--;
                                    rSrcRect.left = rSrcRect.left % 3 *(uiMapTileSize_X / (j+1));
                                    if (!j)
                                        rSrcRect.left += uiMapTileSize_X / 2;

                                    rSrcRect.right = rSrcRect.left + uiMapTileSize_X / 2;


                                    // check y pos
                                    rSrcRect.top = iAutoTile%((int)pow(10.0f, (float)iCount));
                                    rSrcRect.top /= (int)pow(10.0f, (float)iCount-1);
                                    rSrcRect.top--;
                                    rSrcRect.top = rSrcRect.top / 3 *(uiMapTileSize_Y / (i+1));
                                    if (!i)
                                        rSrcRect.top += uiMapTileSize_Y / 2;

                                    rSrcRect.bottom = rSrcRect.top + uiMapTileSize_Y / 2;

                                    // calc draw pos
                                    if (j)
                                        vPosTemp.x = vPosition.x;
                                    else
                                        vPosTemp.x = vPosition.x + uiMapTileSize_X / 2;

                                    if (i)
                                        vPosTemp.y = vPosition.y;
                                    else
                                        vPosTemp.y = vPosition.y + uiMapTileSize_Y / 2;
                                    vPosTemp.z = vPosition.z;

                                    pSprite->Draw(pTexture->GetTexture(), &rSrcRect, NULL, &vPosTemp, GetColor());
                                    iCount++;
                                }
                            }
                        }
                        // if autotile is split up into 2 tiles
                        else
                        {
                            D3DXVECTOR3 vPosTemp = vPosition;
                            iAutoTile /= 10;
                            for (int i = 0; i < 2; i++)
                            {
                                // if autotile is left to right split up
                                if (iAutoTile % 10 == 0)
                                {
                                    if (i == 0)
                                        rSrcRect.left = ((iAutoTile/100-1) % 3) *uiMapTileSize_X;
                                    else
                                        rSrcRect.left = ((iAutoTile/10%10-1) % 3) *uiMapTileSize_X;
                                    rSrcRect.right = rSrcRect.left + uiMapTileSize_X;

                                    if (i == 0)
                                        rSrcRect.top = ((iAutoTile/100-1) / 3) *uiMapTileSize_Y;
                                    else
                                        rSrcRect.top = ((iAutoTile/10%10-1) / 3) *uiMapTileSize_Y;

                                    if (i == 1)
                                        rSrcRect.top += uiMapTileSize_Y/2;
                                    rSrcRect.bottom = rSrcRect.top + uiMapTileSize_Y/2;
                                }
                                // if autotile is top to bottom split up
                                else
                                {
                                    if (i == 0)
                                        rSrcRect.left = ((iAutoTile/100-1) % 3) *uiMapTileSize_X;
                                    else
                                        rSrcRect.left = ((iAutoTile/10%10-1) % 3) *uiMapTileSize_X;

                                    if (i == 1)
                                        rSrcRect.left +=uiMapTileSize_X/2;
                                    rSrcRect.right = rSrcRect.left + uiMapTileSize_X/2;

                                    if (i == 0)
                                        rSrcRect.top = ((iAutoTile/100-1) / 3) * uiMapTileSize_Y;
                                    else
                                        rSrcRect.top = ((iAutoTile/10%10-1) / 3) * uiMapTileSize_Y;

                                    rSrcRect.bottom = rSrcRect.top + uiMapTileSize_Y;
                                }
                                pSprite->Draw(pTexture->GetTexture(), &rSrcRect, NULL, &vPosTemp, GetColor());
                                if (iAutoTile % 10 == 0)
                                    vPosTemp.y += uiMapTileSize_Y/2;
                                else
                                    vPosTemp.x += uiMapTileSize_X/2;
                            }
                        }
                    }
                }
            }
            else    // normal map tile
            {
                if (TileTextureSource *pTexture = pRManager->GetMapTexture(SpriteFiles->GetSpriteFileAt(SPRITE_TYPE_MAP, (UINT)fMapTile)))
                {
                    if (pTexture->GetTexture())   
                        pSprite->Draw(pTexture->GetTexture(), NULL, NULL, &vPosition, GetColor());
                }
            }
            vPosition.x += uiMapTileSize_X;
        }
        vPosition.x = GetPositionX() + startposX * uiMapTileSize_X;
        vPosition.y += uiMapTileSize_Y;
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
}

Layer* Map::GetLayerAtNr(UINT uiLayerNr)
{
    if (m_lLayers.empty() || uiLayerNr > m_lLayers.size() -1)
        return NULL;

    return m_lLayers.at(uiLayerNr);
}

void Map::UpdateMap(const UINT uiCurTime, const UINT uiDiff)
{
    // iterate through layerlist
    for (LayerList::iterator itr = m_lLayers.begin(); itr != m_lLayers.end(); ++itr)
    {
        if ((*itr))
            (*itr)->UpdateLayer(uiCurTime, uiDiff);
    }
}

/*#####
## MapLoadThread
#####*/
MapLoadThread::MapLoadThread(std::string sMapName) : ActiveObject()
{
    m_MapLoadState      = MAP_STATE_NONE;
    m_sMapName          = sMapName;
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "MapLoadThread : ";

    _thread.Resume ();
}

void MapLoadThread::Run()
{
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
        case MAP_STATE_DO_OBJECTS:                    // Load tiles
            result = LoadLayerAndObjects(&sMapDataFromFile);
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
}

MapLoadResult MapLoadThread::LoadDataFromFile(std::string sMapName, std::string *sMapData)
{
    // this open and store the XML file:
    ifstream Data(("Maps/" + sMapName).c_str());
    if(!Data)
        return MAP_RESULT_NO_FILE;

    std::string DataLine;
    while(std::getline(Data,DataLine))
        *sMapData += DataLine;

    return MAP_RESULT_OK;
}

MapLoadResult MapLoadThread::LoadInfo(std::string *sMapData)
{
    MapLoadResult result = MAP_RESULT_NONE;

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    CoInitialize(NULL);

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_OK)
    {
        if (sMapData->length())
            pXMLDom->loadXML(sMapData->c_str());

        IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
        if (pNode)
        {
            pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("MapInfo");
            if (pNode)
            {
                LONG iAttributLength = 0;
                IXMLDOMNamedNodeMapPtr pmAttributes;
                hr = pNode->get_attributes(&pmAttributes);
                if (hr == S_OK)
                {
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
                    result = MAP_RESULT_OK;

                    if (&value)
                        VariantClear(&value);
                }   // if (hr == S_OK)
                else
                    result = MAP_RESULT_CORRUPT_FILE;

                if (pmAttributes)
                    pmAttributes.Release();
            }   // if (pNode)
            else
                result = MAP_RESULT_CORRUPT_FILE;
        }   // if (pNode)
        else
            result = MAP_RESULT_CORRUPT_FILE;

        if (pNode)
            pNode.Release();
    }
    else    // if (hr == S_OK)
        result = MAP_RESULT_FAILED;

    if (pXMLDom)
        pXMLDom.Release();

    CoUninitialize();

    return result;
}

MapLoadResult MapLoadThread::LoadTiles(std::string *sMapData)
{
    MapLoadResult result = MAP_RESULT_NONE;

    std::list<std::string> TileByRowFromFileList;

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    CoInitialize(NULL);

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_OK)
    {
        if (!sMapData->empty())
            pXMLDom->loadXML(sMapData->c_str());

        IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
        if (pNode)
        {
            // get tile map from file
            pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("MapData");
            if (pNode)
            {
                IXMLDOMNodeListPtr pMapNodes;
                hr = pNode->get_childNodes(&pMapNodes);
                if (hr == S_OK)
                {
                    LONG iNodeLength = 0;
                    pMapNodes->get_length(&iNodeLength);
                    IXMLDOMNamedNodeMapPtr pmAttributes;

                    BSTR attributeText = NULL;

                    IXMLDOMNodePtr pNodeTemp = NULL;
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
                        TileByRowFromFileList.push_back(temp);
                    }
                    if (pNodeTemp)
                        pNodeTemp.Release();
                    if (attributeText)
                        SysFreeString(attributeText);
                    if (pmAttributes)
                        pmAttributes.Release();

                    result = MAP_RESULT_OK;
                }
                else
                    result = MAP_RESULT_FAILED;
            }
            else
                result = MAP_RESULT_CORRUPT_FILE;
        }
        else
            result = MAP_RESULT_CORRUPT_FILE;

        if (pNode)
            pNode.Release(); 
    }
    else
        result = MAP_RESULT_FAILED;

    if (pXMLDom)
        pXMLDom.Release();

    CoUninitialize();

    // if tiles are in string, read it out and store as readable vector.
    while(!TileByRowFromFileList.empty())
    {
        std::vector<float> vTempTiles;
        std::list<std::string>::iterator itr = TileByRowFromFileList.begin();

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

        m_v2MapTiles.push_back(vTempTiles);
        TileByRowFromFileList.erase(itr);
        result  = MAP_RESULT_OK;
    }

    return result;
}

MapLoadResult MapLoadThread::LoadLayerAndObjects(std::string *sMapData)
{
    MapLoadResult result = MAP_RESULT_NONE;
    std::vector<std::vector<ObjectReadOut*>> vLayerAndObjects;

    std::list<std::string> TileByRowFromFileList;

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    CoInitialize(NULL);

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_OK)
    {
        if (!sMapData->empty())
            pXMLDom->loadXML(sMapData->c_str());

        IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
        if (pNode)
        {
            // get layer + objects from file
            std::vector<std::vector<WorldObject*>> lLayerData;
            pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("LayerData");
            if (pNode)
            {
                // read out layers
                IXMLDOMNodeListPtr pLayerNodeList;
                hr = pNode->get_childNodes(&pLayerNodeList);
                if (hr == S_OK)
                {
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
                            ObjectReadOut *newObject = new ObjectReadOut();
                            for (LONG k = 0; k < iAttributeLength; k++)
                            {
                                // select layer
                                hr = pmObjectAttributes->get_item(k, &pSelectedAttribute);
                                if (hr == S_FALSE)
                                    continue;

                                pSelectedAttribute->get_nodeValue(&value);
                                switch(k)
                                {
                                    // texture
                                case 0:
                                    newObject->m_TextureID = atoi(_bstr_t(value.bstrVal));
                                    break;
                                    // x pos
                                case 1:
                                    newObject->m_XPos = atoi(_bstr_t(value.bstrVal));
                                    break;
                                    // y pos
                                case 2:
                                    newObject->m_YPos = atoi(_bstr_t(value.bstrVal));
                                    break;
                                default:
                                    break;
                                }
                            }
                            vObjects.push_back(newObject);
                            if (pSelectedAttribute)
                                pSelectedAttribute.Release();
                            if (pmObjectAttributes)
                                pmObjectAttributes.Release();
                            if (&value)
                                VariantClear(&value);
                        }
                        if (pmObjectAttributes)
                            pmObjectAttributes.Release();
                        if (pObjectNode)
                            pObjectNode.Release();

                        vLayerAndObjects.push_back(vObjects);
                    }
                    if (pObjectNodeList)
                        pObjectNodeList.Release();
                    if (pLayerNode)
                        pLayerNode.Release();

                    result = MAP_RESULT_OK;
                }                
                else
                    result = MAP_RESULT_FAILED;

                if (pLayerNodeList)
                    pLayerNodeList.Release();
            }
            else
                result = MAP_RESULT_CORRUPT_FILE;
        }
        else
            result = MAP_RESULT_CORRUPT_FILE;

        if (pNode)
            pNode.Release(); 
    }
    else
        result = MAP_RESULT_FAILED;

    if (pXMLDom)
        pXMLDom.Release();

    CoUninitialize();

    if (result == MAP_RESULT_OK)
    {
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

                    // add world object to temp layer
                    WorldObject *pObject = new WorldObject();
                    if (SpriteFiles *pSpriteFiles = SpriteFiles::Get())
                        pObject->SetTextureSource(pSpriteFiles->GetSpriteFileAt(SPRITE_TYPE_OBJECT, vLayerAndObjects.at(i).at(j)->m_TextureID));

                    pObject->SetPosition(D3DXVECTOR2((float)vLayerAndObjects.at(i).at(j)->m_XPos, (float)vLayerAndObjects.at(i).at(j)->m_YPos));
                    pLayer->AddWorldObject(pObject);
                    delete vLayerAndObjects.at(i).at(j);
                }

                m_lLayers.push_back(pLayer);
            }
        }
    }
    return result;
}

void MapLoadThread::GetMapInfo(MapInfo &MapInfo, MapTiles &MapTiles, LayerList &LayerList)
{
    MapInfo     = m_MapInfo;
    MapTiles    = m_v2MapTiles;
    LayerList   = m_lLayers;
}