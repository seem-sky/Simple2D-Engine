#include "GameDatabase.h"
#include "Logfile.h"
#include <fstream>
#include <msxml2.h>

#import <msxml4.dll>

using namespace std;

/*#####
# GameDatabase
#####*/
GameDatabase::GameDatabase(void) : m_pObjectDatabaseLoad(NULL), m_pSpriteDatabaseLoad(NULL), m_pMapDatabaseLoad(NULL), TSingleton()
{
}

GameDatabase::~GameDatabase(void)
{
    if (m_pObjectDatabaseLoad)
        m_pObjectDatabaseLoad->Kill();

    if (m_pSpriteDatabaseLoad)
        m_pSpriteDatabaseLoad->Kill();

    if (m_pMapDatabaseLoad)
        m_pMapDatabaseLoad->Kill();
}

ANIMATION_TIME GameDatabase::WrapAnimationTimeID(UINT aniID)
{
    switch(aniID)
    {
    case 0:
        return ANIMATION_TIME_VERY_SLOW;
    case 1:
        return ANIMATION_TIME_SLOW;
    case 2:
        return ANIMATION_TIME_NORMAL;
    case 3:
        return ANIMATION_TIME_FAST;
    case 4:
        return ANIMATION_TIME_VERY_FAST;
    default:
        return ANIMATION_TIME_NORMAL;
    }
}

MOVEMENT_SPEED GameDatabase::WrapMovementSpeedID(UINT speedID)
{
    switch(speedID)
    {
    case 0:
        return MOVEMENT_SPEED_VERY_SLOW;
    case 1:
        return MOVEMENT_SPEED_SLOW;
    case 2:
        return MOVEMENT_SPEED_NORMAL;
    case 3:
        return MOVEMENT_SPEED_FAST;
    case 4:
        return MOVEMENT_SPEED_VERY_FAST;
    default:
        return MOVEMENT_SPEED_NORMAL;
    }
}

const ObjectPrototype* GameDatabase::GetObjectPrototype (UINT uiID)
{
    ObjectDatabase::iterator itr = m_ObjectDatabase.find(uiID);
    if (itr == m_ObjectDatabase.end())
        return NULL;

    return &itr->second;
}

void GameDatabase::AddObjectToDatabase(ObjectPrototype proto)
{
    m_ObjectDatabase.insert(make_pair(proto.m_uiID, proto));
}

DATABASE_LOAD_RESULT GameDatabase::LoadDatabase(string sFileName)
{
    // if file not stored in string, load it
    if (m_sFileData.empty())
    {
        // this open and store the XML file:
        ifstream Data(sFileName.c_str());
        if(!Data)
            return DATABASE_LOAD_RESULT_NO_FILE;

        string DataLine;
        while(getline(Data,DataLine))
            m_sFileData += DataLine;
    }

    // start ObjectLoad thread
    if (!m_pObjectDatabaseLoad)
        m_pObjectDatabaseLoad = new ObjectDatabaseLoad(m_sFileData);

    // progress ObjectLoad thread
    else
    {
        if (m_pObjectDatabaseLoad->m_LoadResult != DATABASE_LOAD_RESULT_OK)
            return m_pObjectDatabaseLoad->m_LoadResult;
    }

    // start Sprite load thread
    if (!m_pSpriteDatabaseLoad)
        m_pSpriteDatabaseLoad = new SpriteDatabaseLoad(m_sFileData);

    // progress SpriteLoad thread
    else
    {
        if (m_pSpriteDatabaseLoad->m_LoadResult != DATABASE_LOAD_RESULT_OK)
            return m_pSpriteDatabaseLoad->m_LoadResult;
    }

    // start Map load thread
    if (!m_pMapDatabaseLoad)
        m_pMapDatabaseLoad = new MapDatabaseLoad(m_sFileData);

    // progress MapLoad thread
    else
    {
        if (m_pMapDatabaseLoad->m_LoadResult != DATABASE_LOAD_RESULT_OK)
            return m_pMapDatabaseLoad->m_LoadResult;
    }


    // release ObjectLoad
    if (m_pObjectDatabaseLoad)
    {
        m_pObjectDatabaseLoad->Kill();
        m_pObjectDatabaseLoad = NULL;
    }

    // release SpriteLoad
    if (m_pSpriteDatabaseLoad)
    {
        m_pSpriteDatabaseLoad->Kill();
        m_pSpriteDatabaseLoad = NULL;
    }

    // release MapLoad
    if (m_pMapDatabaseLoad)
    {
        m_pMapDatabaseLoad->Kill();
        m_pMapDatabaseLoad = NULL;
    }

    return DATABASE_LOAD_RESULT_OK;
}

const SpritePrototype* GameDatabase::GetSpriteFile(UINT uiID)
{
    SpriteFilesList::iterator itr = m_StoredSpritesMAP.find(uiID);
    if (itr == m_StoredSpritesMAP.end())
        return NULL;

    return &(*itr).second;
}

std::string GameDatabase::GetFileLocation(SpriteType spriteType)
{
    std::map<SpriteType, std::string>::iterator itr = m_RessourceLocationMap.find(spriteType);
    if (itr == m_RessourceLocationMap.end())
        return NULL;

    return itr->second;
}

/*#####
# ObjectDatabase
#####*/
void ObjectDatabaseLoad::Run()
{
    if (FAILED(CoInitialize(NULL)))
        ERROR_LOG(m_sLogLocationName + "Unable to initialize COM.");

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_FALSE)
        m_LoadResult = DATABASE_LOAD_RESULT_FAILED;

    // search for the file, if it exists
    if (!pXMLDom->loadXML(m_sObjects.c_str()))
        m_LoadResult = DATABASE_LOAD_RESULT_FAILED;

    // checkout ObjectPrototypes
    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Database");
    if (!pNode)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    // checkout ObjectPrototypes
    pNode = pXMLDom->selectSingleNode("Database")->selectSingleNode("ObjectDatabase");
    if (!pNode)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    IXMLDOMNodeListPtr pFileNameNodes;
    hr = pNode->get_childNodes(&pFileNameNodes);
    if (hr == S_FALSE)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    LONG iFileNameLength = 0;
    pFileNameNodes->get_length(&iFileNameLength);
    IXMLDOMNamedNodeMapPtr pmAttributes;

    for (int i = 0; i < iFileNameLength; i++)
    {
        // check objects
        hr = pFileNameNodes->get_item(i, &pNode);
        if (hr == S_FALSE)
            continue;

        hr = pNode->get_attributes(&pmAttributes);
        if (hr == S_FALSE)
            continue;

        // check attributes
        LONG iAttributeLength = 0;
        pmAttributes->get_length(&iAttributeLength);
        IXMLDOMNodePtr pAttribute = NULL;
        VARIANT value;
        VariantInit(&value);
        ObjectPrototype proto;
        UINT uiNonSpecificData = 0;
        BSTR sNodeName;
        for (int j = 0; j < iAttributeLength; j++)
        {
            bool bSuccessRead = false;
            hr = pmAttributes->get_item(j, &pAttribute);
            if (hr == S_OK)
            {
                // check attribute text and name
                hr = pAttribute->get_nodeValue(&value);
                if (hr == S_OK)
                {
                    hr = pAttribute->get_nodeName(&sNodeName);
                    if (hr == S_OK)
                        bSuccessRead = true;
                }
            }

            // if read successs, set data
            if (bSuccessRead)
            {
                std::string sName = _bstr_t(sNodeName);
                proto.SetDataForTypeAt(sName, value);
            }
        }

        // add Object to database
        if (GameDatabase *pDatabase = GameDatabase::Get())
            pDatabase->AddObjectToDatabase(proto);
    }

    m_LoadResult = DATABASE_LOAD_RESULT_OK;
    CoUninitialize();
}
void ObjectPrototype::SetDataForTypeAt(std::string sNodeName, VARIANT value)
{
    if (sNodeName == "ID")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        m_uiID = value.uiVal;
    }
    else if (sNodeName == "Type")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        m_uiType = value.uiVal;
    }
    else if (sNodeName == "TextureID")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        m_uiTextureID = value.uiVal;
    }
    else if (sNodeName == "AniFrequence")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiAnimationFrequence = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "MoveSpeed")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiMoveSpeed = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "HP_min")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiHPmin = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "HP_max")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiHPmax = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "Level_min")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiLevelMin = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "Level_max")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiLevelMax = value.uintVal;
            break;
        default:
            break;
        }
    }
}

/*#####
# SpriteDatabase
#####*/
void SpriteDatabaseLoad::Run()
{
    if (FAILED(CoInitialize(NULL)))
        ERROR_LOG(m_sLogLocationName + "Unable to initialize COM.");

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_FALSE)
        m_LoadResult = DATABASE_LOAD_RESULT_FAILED;

    // search for the file, if it exists
    if (!pXMLDom->loadXML(m_sSprites.c_str()))
        m_LoadResult = DATABASE_LOAD_RESULT_FAILED;

    // checkout SpritePrototypes
    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Database");
    if (!pNode)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    // checkout SpritePrototypes
    pNode = pXMLDom->selectSingleNode("Database")->selectSingleNode("SpriteDatabase");
    if (!pNode)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    IXMLDOMNamedNodeMapPtr pmAttributes;
    hr = pNode->get_attributes(&pmAttributes);
    if (hr == S_FALSE)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    // iterate through sprite locations
    LONG iLocationLength = 0;
    pmAttributes->get_length(&iLocationLength);
    for (LONG i = 0; i < iLocationLength; i++)
    {
        hr = pmAttributes->get_item(i, &pNode);
        if (hr == S_FALSE)
            continue;

        BSTR attributeText = NULL;
        hr = pNode->get_text(&attributeText);
        if (hr == S_FALSE)
            continue;

        // insert location into map
        if (GameDatabase *pDatabase = GameDatabase::Get())
            pDatabase->AddSpriteLocation(std::make_pair<SpriteType, std::string>((SpriteType)i, (std::string)_bstr_t(attributeText)));
    }

    // checkout filenames
    pNode = pXMLDom->selectSingleNode("Database")->selectSingleNode("SpriteDatabase");
    if (!pNode)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    IXMLDOMNodeListPtr pFileNameNodes;
    hr = pNode->get_childNodes(&pFileNameNodes);
    if (hr == S_FALSE)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    LONG iFileNameLength = 0;
    pFileNameNodes->get_length(&iFileNameLength);

    for (int i = 0; i < iFileNameLength; i++)
    {
        // continue if there are no "Tiles" set
        hr = pFileNameNodes->get_item(i, &pNode);
        if (hr == S_FALSE)
            continue;

        hr = pNode->get_attributes(&pmAttributes);
        if (hr == S_FALSE)
            continue;

        // check attributes
        LONG iAttributeLength = 0;
        pmAttributes->get_length(&iAttributeLength);
        IXMLDOMNodePtr pAttribute = NULL;
        VARIANT value;
        VariantInit(&value);
        SpritePrototype proto;
        UINT uiNonSpecificData = 0;
        BSTR sNodeName;
        for (int j = 0; j < iAttributeLength; j++)
        {
            bool bSuccessRead = false;
            hr = pmAttributes->get_item(j, &pAttribute);
            if (hr == S_OK)
            {
                // check attribute text and name
                hr = pAttribute->get_nodeValue(&value);
                if (hr == S_OK)
                {
                    hr = pAttribute->get_nodeName(&sNodeName);
                    if (hr == S_OK)
                        bSuccessRead = true;
                }
            }

            // if read successs, set data
            if (bSuccessRead)
            {
                std::string sName = _bstr_t(sNodeName);
                proto.SetDataForTypeAt(sName, value);
            }
        }

        // add proto to database
        if (GameDatabase *pDatabase = GameDatabase::Get())
            pDatabase->AddSpriteToDatabase(make_pair<UINT, SpritePrototype>(proto.m_uiID, proto));
    }

    m_LoadResult = DATABASE_LOAD_RESULT_OK;
}

void SpritePrototype::SetDataForTypeAt(std::string sNodeName, VARIANT value)
{
    if (sNodeName == "ID")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        m_uiID = value.uiVal;
    }
    else if (sNodeName == "FileName")
        m_sFileName = bstr_t(value);

    else if (sNodeName == "Type")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        m_uiSpriteType = value.uiVal;
    }
    else if (sNodeName == "transparent_color")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UI4);
        m_transparentColor = value.ulVal;
    }
    else if (sNodeName == "spritesX")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
            // count of spritesX
        case SPRITE_TYPE_ANIMATED_OBJECT:
            Type.AnimatedObject.m_uiSpritesX = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "spritesY")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
            // count of spritesX
        case SPRITE_TYPE_ANIMATED_OBJECT:
            Type.AnimatedObject.m_uiSpritesY = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "boundingXBegin")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
        case SPRITE_TYPE_OBJECT:
        case SPRITE_TYPE_ANIMATED_OBJECT:
            Type.Object.m_uiBoundingXBegin = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "boundingYBegin")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
        case SPRITE_TYPE_OBJECT:
        case SPRITE_TYPE_ANIMATED_OBJECT:
            Type.Object.m_uiBoundingYBegin = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "boundingXRange")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
        case SPRITE_TYPE_OBJECT:
        case SPRITE_TYPE_ANIMATED_OBJECT:
            Type.Object.m_uiBoundingXRange = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "boundingYRange")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
        case SPRITE_TYPE_OBJECT:
        case SPRITE_TYPE_ANIMATED_OBJECT:
            Type.Object.m_uiBoundingYRange = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "passability")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
            // passability
        case SPRITE_TYPE_TILE:
        case SPRITE_TYPE_AUTOTILE:
            Type.Tile.m_uiPassable = value.uintVal;
            break;
        default:
            break;
        }
    }
    else if (sNodeName == "terraintype")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        switch(m_uiSpriteType)
        {
            // terrain type
        case SPRITE_TYPE_TILE:
        case SPRITE_TYPE_AUTOTILE:
            Type.Tile.m_uiTerrainType = value.uintVal;
            break;
        }
    }
}

void MapDatabaseLoad::Run()
{
    if (FAILED(CoInitialize(NULL)))
        ERROR_LOG(m_sLogLocationName + "Unable to initialize COM.");

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_FALSE)
        m_LoadResult = DATABASE_LOAD_RESULT_FAILED;

    // search for the file, if it exists
    if (!pXMLDom->loadXML(m_sMaps.c_str()))
        m_LoadResult = DATABASE_LOAD_RESULT_FAILED;

    // checkout ObjectPrototypes
    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Database");
    if (!pNode)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    // checkout ObjectPrototypes
    pNode = pXMLDom->selectSingleNode("Database")->selectSingleNode("MapDatabase");
    if (!pNode)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    IXMLDOMNodeListPtr pFileNameNodes;
    hr = pNode->get_childNodes(&pFileNameNodes);
    if (hr == S_FALSE)
    {
        m_LoadResult = DATABASE_LOAD_RESULT_CORRUPT_FILE;
        return;
    }

    LONG iFileNameLength = 0;
    pFileNameNodes->get_length(&iFileNameLength);
    IXMLDOMNamedNodeMapPtr pmAttributes;

    for (int i = 0; i < iFileNameLength; i++)
    {
        // check objects
        hr = pFileNameNodes->get_item(i, &pNode);
        if (hr == S_FALSE)
            continue;

        hr = pNode->get_attributes(&pmAttributes);
        if (hr == S_FALSE)
            continue;

        // check attributes
        LONG iAttributeLength = 0;
        pmAttributes->get_length(&iAttributeLength);
        IXMLDOMNodePtr pAttribute = NULL;
        VARIANT value;
        VariantInit(&value);
        MapPrototype proto;
        UINT uiNonSpecificData = 0;
        BSTR sNodeName;
        for (int j = 0; j < iAttributeLength; j++)
        {
            bool bSuccessRead = false;
            hr = pmAttributes->get_item(j, &pAttribute);
            if (hr == S_OK)
            {
                // check attribute text and name
                hr = pAttribute->get_nodeValue(&value);
                if (hr == S_OK)
                {
                    hr = pAttribute->get_nodeName(&sNodeName);
                    if (hr == S_OK)
                        bSuccessRead = true;
                }
            }

            // if read successs, set data
            if (bSuccessRead)
            {
                std::string sName = _bstr_t(sNodeName);
                proto.SetDataForTypeAt(sName, value);
            }
        }

        // add Object to database
        if (GameDatabase *pDatabase = GameDatabase::Get())
            pDatabase->AddMapToDatabase(proto);
    }

    m_LoadResult = DATABASE_LOAD_RESULT_OK;
    CoUninitialize();
}

void GameDatabase::AddMapToDatabase(MapPrototype proto)
{
    m_MapDatabase.insert(make_pair(proto.m_uiID, proto));
}

void MapPrototype::SetDataForTypeAt(std::string sNodeName, VARIANT value)
{
    if (sNodeName == "ID")
    {
        VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
        m_uiID = value.uiVal;
    }
    else if (sNodeName == "File")
    {
        m_sFileName = _bstr_t(value.bstrVal);
    }
}