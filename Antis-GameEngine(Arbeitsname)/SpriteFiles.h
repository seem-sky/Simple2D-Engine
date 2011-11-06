#ifndef SPRITE_FILES_H
#define SPRITE_FILES_H

#include "Logfile.h"
#include <vector>

typedef std::vector<std::string> SpriteFilesList;

enum SpriteType
{
    SPRITE_TYPE_MAP,
    SPRITE_TYPE_AUTOTILE,
    SPRITE_TYPE_OBJECT,
};

enum SpriteFileReadResult
{
    SPRITE_FILE_OK,
    SPRITE_FILE_CORRUPT_FILE,
    SPRITE_FILE_NO_FILE,
    SPRITE_FILE_FAILED,
};

class SpriteFiles : public TSingleton<SpriteFiles>
{
public:
    SpriteFiles(void);

    SpriteFileReadResult LoadSpriteDataFromFile(SpriteType SpriteType);

    std::string GetStringForSpriteType(SpriteType SpriteType);

    void ChangeSpriteWithSprite(SpriteType SpriteType, unsigned int uiAt, unsigned int uiWith);

    // return string of the stored texture, if not exist return empty string.
    std::string GetSpriteFileAt(SpriteType SpriteType, unsigned int uiAtPos);

private:
    // stores the file names of all map sprites
    SpriteFilesList m_vMapFiles;
    // stores the file names of all autotiles
    SpriteFilesList m_vAutoTileFiles;
    // stores the file names of all character sprites
    SpriteFilesList m_vCharacterFiles;

    std::string m_sLogLocationName;
    CLogfile *m_pLogfile;
};
#endif;