#ifndef GAME_DATABASE_H
#define GAME_DATABASE_H

#include "Singleton.h"
#include "Thread.h"
#include <map>

enum DATABASE_LOAD_RESULT
{
    DATABASE_LOAD_RESULT_OK,
    DATABASE_LOAD_RESULT_FAILED,
    DATABASE_LOAD_RESULT_IN_PROGRESS,
    DATABASE_LOAD_RESULT_CORRUPT_FILE,
    DATABASE_LOAD_RESULT_NO_FILE,
};

/*#####
# ObjectDatabase
#####*/
enum OBJECT_TYPE
{
    OBJECT_TYPE_MAP_OBJECT,
    OBJECT_TYPE_NPC,
};

struct ObjectPrototype
{
    UINT m_uiID;
    UINT m_uiType;
    UINT m_uiTextureID;

    union
    {
        // 0 = MapObject
        struct sMapObject
        {
        }MapObject;

        // 1 = NPC
        struct sNPC : sMapObject
        {
            UINT m_uiAnimationFrequence;
            UINT m_uiHPmin;
            UINT m_uiHPmax;
            UINT m_uiLevelMin;
            UINT m_uiLevelMax;
            UINT m_uiMoveSpeed;
        }NPC;
    }ObjectType;

    ObjectPrototype() : m_uiID(0), m_uiType(0)
    {
        memset(&ObjectType, 0, sizeof(ObjectType));
    }

    void SetDataForTypeAt(UINT uiAt, UINT uiData);
};

struct ObjectDatabaseLoad : public ActiveObject
{
    ObjectDatabaseLoad(std::string sData) : m_sObjects(sData), m_LoadResult(DATABASE_LOAD_RESULT_IN_PROGRESS), ActiveObject()
    {
        m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "ObjectDatabaseLoad : ";

        _thread.Resume ();
    }

    std::string m_sObjects;
    DATABASE_LOAD_RESULT m_LoadResult;
    std::string m_sLogLocationName;

    // load objects from data-string
    void Run();
};

/*#####
# SpriteDatabase
#####*/
enum SpriteType
{
    SPRITE_TYPE_TILE,
    SPRITE_TYPE_AUTOTILE,
    SPRITE_TYPE_OBJECT,
    SPRITE_TYPE_ANIMATED_OBJECT,
};

// passability flag, if nothing set, its unpassable
enum PassabilityFlag
{
    PASSABLE_NONE           = 0x00,
    PASSABLE_UP             = 0x01,
    PASSABLE_DOWN           = 0x02,
    PASSABLE_RIGHT          = 0x04,
    PASSABLE_LEFT           = 0x08,
    PASSABLE_UP_TO_DOWN     = PASSABLE_UP+PASSABLE_DOWN,
    PASSABLE_LEFT_TO_RIGHT  = PASSABLE_LEFT+PASSABLE_RIGHT,
    PASSABLE_ALL            = PASSABLE_LEFT+PASSABLE_RIGHT+PASSABLE_DOWN+PASSABLE_UP,
};

struct SpritePrototype
{
    SpritePrototype() : m_uiSpriteType(SPRITE_TYPE_TILE), m_uiID(0), m_transparentColor(0)
    {
        memset(&Type, 0, sizeof(Type));
    }
    // set data at specific position to value
    void SetDataForTypeAt(UINT uiAt, UINT value);

    UINT m_uiSpriteType;
    UINT m_uiID;
    DWORD m_transparentColor;
    // stores texture name (WITHOUT path).
    std::string m_sFileName;
    union                           // different sprite types have different attributes
    {
        // SpriteTile = 0
        struct SpriteTile
        {
            UINT m_uiPassable;      // 0 passability
            UINT m_uiTerrainType;   // 1 terrain type
        } Tile;

        // SpriteAutoTile = 1
        struct SpriteAutoTile : SpriteTile
        {
        } AutoTile;

        // SpriteObject = 2
        struct SpriteObject
        {
        } Object;

        // SpriteAnimatedObject = 3
        struct SpriteAnimatedObject
        {
            UINT m_uiSpritesX;
            UINT m_uiSpritesY;
        } AnimatedObject;
    } Type;
};

typedef std::map<UINT, SpritePrototype> SpriteFilesList;

struct SpriteDatabaseLoad : public ActiveObject
{
    SpriteDatabaseLoad(std::string sData) : m_sSprites(sData), m_LoadResult(DATABASE_LOAD_RESULT_IN_PROGRESS), ActiveObject()
    {
        m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "SpriteDatabaseLoad : ";

        _thread.Resume ();
    }

    std::string m_sSprites;
    DATABASE_LOAD_RESULT m_LoadResult;
    std::string m_sLogLocationName;

    // load sprites from data-string
    void Run();
};

/*#####
# GameDatabase
#####*/
typedef std::map<UINT, ObjectPrototype> ObjectDatabase;

enum ANIMATION_TIME
{
    ANIMATION_TIME_VERY_SLOW    = 1000,
    ANIMATION_TIME_SLOW         = 500,
    ANIMATION_TIME_NORMAL       = 250,
    ANIMATION_TIME_FAST         = 125,
    ANIMATION_TIME_VERY_FAST    = 65,
};

enum MOVEMENT_SPEED
{
    MOVEMENT_SPEED_VERY_SLOW    = 1,
    MOVEMENT_SPEED_SLOW         = 2,
    MOVEMENT_SPEED_NORMAL       = 4,
    MOVEMENT_SPEED_FAST         = 8,
    MOVEMENT_SPEED_VERY_FAST    = 16,
};

enum DIRECTION
{
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_NONE,
};

class GameDatabase : public TSingleton<GameDatabase>
{
    friend struct ObjectDatabaseLoad;
    friend struct SpriteDatabaseLoad;
public:
    GameDatabase(void);
    ~GameDatabase(void);

    /*#####
    # System
    #####*/
    // load complete Databas; hand over complete path + filename
    DATABASE_LOAD_RESULT LoadDatabase(std::string sFileName);
    static ANIMATION_TIME WrapAnimationTimeID(UINT aniID);
    static MOVEMENT_SPEED WrapMovementSpeedID(UINT speedID);

    /*#####
    # ObjectDatabase
    #####*/
    //returns ObjectPrototype with ID; if not exist, return NULL
    const ObjectPrototype* GetObjectPrototype (UINT uiID);

    /*#####
    # SpriteDatabase
    #####*/
    // return prototype of stored sprite;
    const SpritePrototype* GetSpriteFile(UINT uiID);
    // return string of the location of ressources
    std::string GetFileLocation(SpriteType spriteType);

private:
    /*#####
    # System
    #####*/
    // stores xml file; only set while loading database
    std::string m_sFileData;
    // standard = NULL; only set if loading objects from database file
    ObjectDatabaseLoad *m_pObjectDatabaseLoad;
    // standard = NULL; only set if loading sprites from database file
    SpriteDatabaseLoad *m_pSpriteDatabaseLoad;

    /*#####
    # ObjectDatabase
    #####*/
    // stores all Object Prototypes
    ObjectDatabase m_ObjectDatabase;

    // add new ObjectPrototype to ObjectDatabase
    void AddObjectToDatabase(ObjectPrototype proto);

    /*#####
    # SpriteDatabase
    #####*/
    // stores all prototypes by type and name
    SpriteFilesList m_StoredSpritesMAP;
    // stores the file location of all types
    std::map<SpriteType, std::string> m_RessourceLocationMap;

    // add new Sprite Location
    void AddSpriteLocation(std::pair<SpriteType, std::string> pair) { m_RessourceLocationMap.insert(pair); }
    // add new Sprite to Database
    void AddSpriteToDatabase(std::pair<UINT, SpritePrototype> pair) { m_StoredSpritesMAP.insert(pair); }
};
#endif;