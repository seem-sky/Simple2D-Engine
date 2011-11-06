#include "SpriteFiles.h"
#include <msxml2.h>

#import <msxml4.dll>

SpriteFiles::SpriteFiles()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "SpriteFiles : ";
    m_pLogfile          = CLogfile::Get();
}

SpriteFileReadResult SpriteFiles::LoadSpriteDataFromFile(SpriteType SpriteType)
{
    SpriteFileReadResult result;

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT hr;

    CoInitialize(NULL);

    hr = pXMLDom.CreateInstance(__uuidof(DOMDocument40));
    if (hr == S_OK)
    {
        // search for the file, if it exists
        VARIANT_BOOL bResult;
        bResult = pXMLDom->load(GetStringForSpriteType(SpriteType).c_str());
        if (bResult)
        {
            // check file location
            IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("SpriteData");
            if (pNode)
            {
                IXMLDOMNamedNodeMapPtr pmAttributes;
                hr = pNode->get_attributes(&pmAttributes);
                if (hr == S_FALSE)
                    return SPRITE_FILE_CORRUPT_FILE;

                hr = pmAttributes->get_item(0, &pNode);
                if (hr == S_FALSE)
                    return SPRITE_FILE_CORRUPT_FILE;

                BSTR attributeText = NULL;
                hr = pNode->get_text(&attributeText);
                if (hr == S_FALSE)
                    return SPRITE_FILE_CORRUPT_FILE;

                std::string sFileLocation = _bstr_t(attributeText);

                // checkout filenames
                pNode = pXMLDom->selectSingleNode("SpriteData");
                if (pNode)
                {
                    IXMLDOMNodeListPtr pFileNameNodes;
                    hr = pNode->get_childNodes(&pFileNameNodes);
                    if (hr == S_FALSE) 
                        return SPRITE_FILE_CORRUPT_FILE;

                    LONG iFileNameLength = 0;
                    pFileNameNodes->get_length(&iFileNameLength);

                    // first pic is empty
                    SpriteFilesList vFiles;
                    vFiles.push_back("");
                    for (int i = 0; i < iFileNameLength; i++)
                    {
                        // continue if there are no "Tiles" set
                        hr = pFileNameNodes->get_item(i, &pNode);
                        if (hr == S_FALSE)
                            continue;

                        hr = pNode->get_attributes(&pmAttributes);
                        if (hr == S_FALSE)
                            continue;

                        hr = pmAttributes->get_item(0, &pNode);
                        if (hr == S_FALSE)
                            continue;

                        hr = pNode->get_text(&attributeText);
                        if (hr == S_FALSE)
                            continue;

                        std::string sTemp = _bstr_t(attributeText);
                        vFiles.push_back(sFileLocation+sTemp);
                    }

                    switch(SpriteType)
                    {
                    case SPRITE_TYPE_MAP:
                        m_vMapFiles = vFiles;
                        break;
                    case SPRITE_TYPE_AUTOTILE:
                        m_vAutoTileFiles = vFiles;
                        break;
                    case SPRITE_TYPE_OBJECT:
                        m_vCharacterFiles = vFiles;
                        break;
                    default: break;
                    }

                    // release
                    if (attributeText)
                        SysFreeString(attributeText);

                    if (pmAttributes)
                        pmAttributes.Release();

                    result = SPRITE_FILE_OK;
                }
                else
                    result = SPRITE_FILE_CORRUPT_FILE;
            }
            else
                result = SPRITE_FILE_CORRUPT_FILE;

            // release
            if (pNode)
                pNode.Release();
        }
        else
            result = SPRITE_FILE_NO_FILE;
    }
    else
        result = SPRITE_FILE_FAILED;

    // release
    if (pXMLDom)
        pXMLDom.Release();

    CoUninitialize();

    switch(result)
    {
    case SPRITE_FILE_FAILED:
        BASIC_LOG(m_sLogLocationName + "Undefined Error while loading map sprite files.");
        break;
    case SPRITE_FILE_NO_FILE:
        BASIC_LOG(m_sLogLocationName + "Unable to find " + GetStringForSpriteType(SpriteType).c_str() + ".");
        break;
    case SPRITE_FILE_CORRUPT_FILE:
        BASIC_LOG(m_sLogLocationName + GetStringForSpriteType(SpriteType).c_str() + " is a corrupt file. Check syntax or rebuild it.");
        break;
    case SPRITE_FILE_OK:
        BASIC_LOG(m_sLogLocationName + "Successfully load content from " + GetStringForSpriteType(SpriteType).c_str() + ".");
        break;
    default: break;
    }

    return result;
}

std::string SpriteFiles::GetStringForSpriteType(SpriteType SpriteType)
{
    switch(SpriteType)
    {
    case SPRITE_TYPE_MAP:
        return "Game/MapSprites.xml";
    case SPRITE_TYPE_AUTOTILE:
        return "Game/AutoTiles.xml";
    case SPRITE_TYPE_OBJECT:
        return "Game/CharacterSprites.xml";
    default:
        return "";
    }
}

void SpriteFiles::ChangeSpriteWithSprite(SpriteType SpriteType, unsigned int uiAt, unsigned int uiWith)
{
    std::string sTemp;
    switch(SpriteType)
    {
    case SPRITE_TYPE_MAP:
        // if new or cur Texture = NULL return
        if (m_vMapFiles.empty() || m_vMapFiles.size()-1 < uiAt || m_vMapFiles.size()-1 < uiWith)
            return;

        sTemp                   = m_vMapFiles.at(uiAt);
        m_vMapFiles.at(uiAt)    = m_vMapFiles.at(uiWith);
        m_vMapFiles.at(uiWith)  = sTemp;
        break;
    case SPRITE_TYPE_AUTOTILE:
        // if new or cur Texture = NULL return
        if (m_vAutoTileFiles.empty() || m_vAutoTileFiles.size()-1 < uiAt || m_vAutoTileFiles.size()-1 < uiWith)
            return;

        sTemp                           = m_vAutoTileFiles.at(uiAt);
        m_vAutoTileFiles.at(uiAt)       = m_vAutoTileFiles.at(uiWith);
        m_vAutoTileFiles.at(uiWith)     = sTemp;
        break;
    case SPRITE_TYPE_OBJECT:
        // if new or cur Texture = NULL return
        if (m_vCharacterFiles.empty() || m_vCharacterFiles.size()-1 < uiAt || m_vCharacterFiles.size()-1 < uiWith)
            return;

        sTemp                       = m_vCharacterFiles.at(uiAt);
        m_vCharacterFiles.at(uiAt)  = m_vCharacterFiles.at(uiWith);
        m_vCharacterFiles.at(uiWith)= sTemp;
        break;
    }
}

std::string SpriteFiles::GetSpriteFileAt(SpriteType SpriteType, unsigned int uiAtPos)
{
    switch(SpriteType)
    {
    case SPRITE_TYPE_MAP:
        if (!m_vMapFiles.empty() && uiAtPos <= m_vMapFiles.size()-1)
            return m_vMapFiles.at(uiAtPos);
        break;
    case SPRITE_TYPE_AUTOTILE:
        if (!m_vAutoTileFiles.empty() && uiAtPos <= m_vAutoTileFiles.size()-1)
            return m_vAutoTileFiles.at(uiAtPos);
        break;
    case SPRITE_TYPE_OBJECT:
        if (!m_vCharacterFiles.empty() && uiAtPos <= m_vCharacterFiles.size()-1)
            return m_vCharacterFiles.at(uiAtPos);
        break;
    }

    return "";
}