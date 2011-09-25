#ifndef SPRITE_FILES_H
#define SPRITE_FILES_H

#include "Singleton.h"
#include "xmlParser.h"

typedef std::vector<std::string> SpriteFilesList;

enum SpriteType
{
    SPRITE_TYPE_MAP,
    SPRITE_TYPE_CHARACTERS,
};

enum SpriteFileReadResult
{
    SPRITE_FILE_OK,
    SPRITE_FILE_CORRUPT_FILE,
    SPRITE_FILE_NO_FILE,
};

struct SpriteFiles
{
    // the location where the files are stored
    std::string m_sFileLocation;

    // stores the file names of all sprites stored in MapSprites.xml
    SpriteFilesList m_vSpriteFiles;

    SpriteFileReadResult LoadSpriteDataFromFile(SpriteType SpriteType)
    {
        // clear SpriteFiles
        if (!m_vSpriteFiles.empty())
            m_vSpriteFiles.clear();

        // search for the file, if it exists
        FILE *Data;
        fopen_s(&Data, GetStringForSpriteType(SpriteType).c_str(), "r");
        if(!Data)
            return SPRITE_FILE_NO_FILE;

        fclose(Data);

        // this open and parse the XML file:
        XMLNode xMainNode = XMLNode::openFileHelper(GetStringForSpriteType(SpriteType).c_str());
        XMLNode xNode = xMainNode.getChildNode("MapSpriteData");
        if (xNode.isAttributeSet("Location"))
            m_sFileLocation = xNode.getAttribute("Location");
        else
            return SPRITE_FILE_CORRUPT_FILE;

        int n = xNode.nChildNode("MapSprite");
        for (int i = 0; i < n; i++)
        {
            // if has no "FileName", continue
            if (!xNode.getChildNode("MapSprite", i).isAttributeSet("FileName"))
                continue;

            m_vSpriteFiles.push_back(xNode.getChildNode("MapSprite", i).getAttribute("FileName"));
        }
        return SPRITE_FILE_OK;
    }

    std::string GetStringForSpriteType(SpriteType SpriteType)
    {
        switch(SpriteType)
        {
        case SPRITE_TYPE_MAP:
            return "Game/MapSprites.xml";
        case SPRITE_TYPE_CHARACTERS:
            return "Game/CharacterSprites.xml";
        }
        return "";
    }
};
#endif;