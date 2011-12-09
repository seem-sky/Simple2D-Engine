#include "GameDatabase.h"
#include "Logfile.h"
#include <fstream>
#include <msxml2.h>

#import <msxml4.dll>

using namespace std;

/*#####
# GameDatabase
#####*/
GameDatabase::GameDatabase(void) : TSingleton()
{
    m_pObjectDatabaseLoad   = NULL;
    m_pSpriteDatabaseLoad   = NULL;
}

GameDatabase::~GameDatabase(void)
{
    if (m_pObjectDatabaseLoad)
        m_pObjectDatabaseLoad->Kill();

    if (m_pSpriteDatabaseLoad)
        m_pSpriteDatabaseLoad->Kill();
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
        for (int j = 0; j < iAttributeLength; j++)
        {
            bool bSuccessRead = false;
            hr = pmAttributes->get_item(j, &pAttribute);
            if (hr == S_OK)
            {
                // check attribute text
                hr = pAttribute->get_nodeValue(&value);
                if (hr == S_OK)
                    bSuccessRead = true;
            }

            // set attributes of object
            switch(j)
            {
            case 0:     // set ID
                if (!bSuccessRead)
                    continue;

                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                proto.m_uiID = value.uiVal;
                break;
            case 1:     // set type
                if (!bSuccessRead)
                    continue;

                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                proto.m_uiType = value.uiVal;
                break;
            case 2:     // set texture ID
                if (!bSuccessRead)
                    continue;

                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                proto.m_uiTextureID = value.uiVal;
                break;
            default:    // set other values
                if (bSuccessRead)
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    proto.SetDataForTypeAt(uiNonSpecificData, value.uiVal);
                }
                uiNonSpecificData++;
                break;
            }
        }

        // add Object to database
        if (GameDatabase *pDatabase = GameDatabase::Get())
            pDatabase->AddObjectToDatabase(proto);
    }

    m_LoadResult = DATABASE_LOAD_RESULT_OK;
    CoUninitialize();
}

void ObjectPrototype::SetDataForTypeAt(UINT uiAt, UINT uiData)
{
    switch(uiAt)
    {
    // animation frequence
    case 0:
        switch(m_uiType)
        {
        case OBJECT_TYPE_MAP_OBJECT:
        case OBJECT_TYPE_NPC:
            ObjectType.MapObject.m_uiAnimationFrequence = uiData;
            break;
        default:
            break;
        }
        break;
    // hp min
    case 1:
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiHPmin = uiData;
            break;
        default:
            break;
        }
        break;
    // hp max
    case 2:
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiHPmax = uiData;
            break;
        default:
            break;
        }
        break;
    // level min
    case 3:
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiLevelMin = uiData;
            break;
        default:
            break;
        }
        break;
    // level max
    case 4:
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiLevelMax = uiData;
            break;
        default:
            break;
        }
        break;
    // movement speed
    case 5:
        switch(m_uiType)
        {
        case OBJECT_TYPE_NPC:
            ObjectType.NPC.m_uiSpeed = uiData;
            break;
        default:
            break;
        }
        break;
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
        for (int j = 0; j < iAttributeLength; j++)
        {
            bool bSuccessRead = false;
            hr = pmAttributes->get_item(j, &pAttribute);
            if (hr == S_OK)
            {
                // check attribute text
                hr = pAttribute->get_nodeValue(&value);
                if (hr == S_OK)
                    bSuccessRead = true;
            }

            // set attributes of object
            switch(j)
            {
            case 0:     // set ID
                if (!bSuccessRead)
                    continue;

                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                proto.m_uiID = value.uiVal;
                break;
            case 1:     // set filename
                if (!bSuccessRead)
                    continue;

                proto.m_sFileName = bstr_t(value);
                break;
            case 2:     // set sprite type
                if (!bSuccessRead)
                    continue;

                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                proto.m_uiSpriteType = value.uiVal;
                break;
            case 3:     // set transparent color
                if (!bSuccessRead)
                    continue;

                VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UI4);
                proto.m_transparentColor = value.ulVal;
                break;
            default:    // set other values
                if (bSuccessRead)
                {
                    VariantChangeType(&value, &value, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    proto.SetDataForTypeAt(j-4, value.uiVal);
                }
                uiNonSpecificData++;
                break;
            }
        }

        // add proto to database
        if (GameDatabase *pDatabase = GameDatabase::Get())
            pDatabase->AddSpriteToDatabase(make_pair<UINT, SpritePrototype>(proto.m_uiID, proto));
    }

    m_LoadResult = DATABASE_LOAD_RESULT_OK;
}

void SpritePrototype::SetDataForTypeAt(UINT uiAt, UINT value)
{
    // store data at specific position
    switch(uiAt)
    {
    case 0:
        switch(m_uiSpriteType)
        {
            // passability
        case SPRITE_TYPE_TILE:
        case SPRITE_TYPE_AUTOTILE:
            Type.Tile.m_uiPassable = value;
            break;

            // count of spritesX
        case SPRITE_TYPE_AIMATED_OBJECT:
            Type.AnimatedObject.m_uiSpritesX = value;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch(m_uiSpriteType)
        {
            // terrain type
        case SPRITE_TYPE_TILE:
        case SPRITE_TYPE_AUTOTILE:
            Type.Tile.m_uiTerrainType = value;
            break;

            // count of spritesX
        case SPRITE_TYPE_AIMATED_OBJECT:
            Type.AnimatedObject.m_uiSpritesY = value;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}