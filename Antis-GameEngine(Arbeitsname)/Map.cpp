#include "Map.h"
#include <msxml2.h>
#include "Time.h"

#import <msxml4.dll>

Map::Map(void)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Map : ";
    m_pLogfile          = CLogfile::Get();
    m_pSpriteFiles      = SpriteFiles::Get();

    m_v3Position.x = 0;
    m_v3Position.y = 0;
    m_v3Position.z = 0;

    m_MapLoadState = MAP_STATE_NONE;

    AddNewWorldObject(m_pSpriteFiles->GetSpriteFileAt(SPRITE_TYPE_OBJECT, 1), 5, 5);
}

Map::~Map(void)
{
    // delete all world objects
    for (ObjectList::iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
    {
        if (*itr)
            delete *itr;
    }

    m_lObjects.clear();
}

MapLoadResult Map::LoadNewMap(std::string sMapName)
{
    MapLoadResult result = MAP_RESULT_NONE;

    switch(m_MapLoadState)
    {
    case MAP_STATE_NONE:                        // Load data from file
        LoadDataFromFile(sMapName);
        if (result != MAP_RESULT_IN_PROGRESS)
            m_MapLoadState = MAP_STATE_DO_INFO;
        break;
    case MAP_STATE_DO_INFO:                     // Load map info
        result = LoadInfo();
        if (result != MAP_RESULT_IN_PROGRESS)
            m_MapLoadState = MAP_STATE_DO_TILES;
        break;
    case MAP_STATE_DO_TILES:                    // Load tiles
        result = LoadTiles();
        if (result != MAP_RESULT_IN_PROGRESS)
        {
            m_MapLoadState = MAP_STATE_DONE;
            m_sMapDataFromFile.clear();
        }
        break;
    default:
        break;
    }

    switch(result)
    {
    case MAP_RESULT_CORRUPT_FILE:                   // if map file has syntax mistake or info missing
        ERROR_LOG(m_sLogLocationName + sMapName + " is a corrupt file. Check syntax or rebuild it.");
        break;
    case MAP_RESULT_NO_FILE:                        // if unable to find file
        ERROR_LOG(m_sLogLocationName + "Unable to find " + sMapName + ". Check filename or directory.");
        break;
    case MAP_RESULT_FAILED:
        ERROR_LOG(m_sLogLocationName + sMapName + " causes an undefined error. Check syntax or rebuild it.");
        break;
    case MAP_RESULT_OK:                             // if result = OK and MapState = Done write into log
        if (m_MapLoadState == MAP_STATE_DONE)
        {
            BASIC_LOG(m_sLogLocationName + "Successfully load " + sMapName + ".");
            return MAP_RESULT_DONE;
        }
        break;
    default:
        break;
    }

    return result;
}

MapLoadResult Map::LoadDataFromFile(std::string sMapName)
{
    // this open and store the XML file:
    ifstream Data(("Maps/" + sMapName).c_str());
    if(!Data)
        return MAP_RESULT_NO_FILE;

    std::string DataLine;
    while(std::getline(Data,DataLine))
        m_sMapDataFromFile += DataLine;

    m_TileByRowFromFileList.clear();

    return MAP_RESULT_OK;
}

MapLoadResult Map::LoadInfo()
{
    MapLoadResult result = MAP_RESULT_NONE;

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    CoInitialize(NULL);

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_OK)
    {
        if (m_sMapDataFromFile.length())
            pXMLDom->loadXML(m_sMapDataFromFile.c_str());

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

MapLoadResult Map::LoadTiles()
{
    MapLoadResult result = MAP_RESULT_NONE;

    // init time
    LONGLONG iTime = 0, iTimeBefore = 0;
    if (CTime *time = CTime::Get())
    {
        iTime = time->GetCurTime();
        iTimeBefore = iTime;
    }

    // if empty, parse data
    if (m_TileByRowFromFileList.empty())
    {
        // this parse the XML file:
        MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
        HRESULT hr;

        CoInitialize(NULL);

        hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
        if (hr == S_OK)
        {
            if (!m_sMapDataFromFile.empty())
                pXMLDom->loadXML(m_sMapDataFromFile.c_str());

            IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
            if (pNode)
            {
                pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("MapData");
                if (pNode)
                {
                    IXMLDOMNodeListPtr pMapTileNodes;
                    hr = pNode->get_childNodes(&pMapTileNodes);
                    if (hr == S_OK)
                    {
                        LONG iMapTileLength = 0;
                        pMapTileNodes->get_length(&iMapTileLength);
                        IXMLDOMNamedNodeMapPtr pmAttributes;

                        VARIANT value;
                        VariantInit(&value);
                        BSTR attributeText = NULL;

                        IXMLDOMNodePtr pNodeTemp = NULL;
                        for (int i = m_v2MapTiles.size(); i < iMapTileLength; i++)
                        {
                            // continue if there are no "Tiles" set
                            hr = pMapTileNodes->get_item(i, &pNodeTemp);
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
                            m_TileByRowFromFileList.push_back(temp);
                        }
                        result = MAP_RESULT_IN_PROGRESS;

                        if (pNodeTemp)
                            pNodeTemp.Release();
                        if (&value)
                            VariantClear(&value);
                        if (attributeText)
                            SysFreeString(attributeText);
                        if (pmAttributes)
                            pmAttributes.Release();
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
    }
    // if tiles are in string, read it out and store as readable vector.
    else
    {
        while(!m_TileByRowFromFileList.empty())
        {
            std::vector<float> vTempTiles;
            std::list<std::string>::iterator itr = m_TileByRowFromFileList.begin();

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
            m_TileByRowFromFileList.erase(itr);
        
            // return if time over 100msec and continue next time
            if (CTime *time = CTime::Get())
            {
                iTime = time->GetCurTime();

                if (iTime - iTimeBefore > time->GetFrenquency() / 10)
                {
                    result = MAP_RESULT_IN_PROGRESS;
                    break;
                }

                iTimeBefore = iTime;
            }
            result  = MAP_RESULT_OK;
        }
    }

    return result;
}

WorldObject* Map::AddNewWorldObject(std::string sFileName, int XPos, int YPos)
{
    WorldObject *pNewObject = new WorldObject();
    pNewObject->SetPosition(D3DXVECTOR2((float)XPos, (float)YPos));
    pNewObject->SetTextureSource(sFileName);
    m_lObjects.push_back(pNewObject);
    return pNewObject;
}