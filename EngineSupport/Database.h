#ifndef DATABASE_H
#define DATABASE_H

#include "XML_Reader.h"
#include "Singleton.h"
#include <list>
#include "Point.h"

// passability flag, if nothing set, its unpassable
enum PassabilityFlag
{
    PASSABLE_NONE           = 0x00,
    PASSABLE_UP             = 0x01,
    PASSABLE_DOWN           = 0x02,
    PASSABLE_RIGHT          = 0x04,
    PASSABLE_LEFT           = 0x08,
    PASSABLE_UP_TO_DOWN     = PASSABLE_UP|PASSABLE_DOWN,
    PASSABLE_LEFT_TO_RIGHT  = PASSABLE_LEFT|PASSABLE_RIGHT,
    PASSABLE_ALL            = PASSABLE_LEFT|PASSABLE_RIGHT|PASSABLE_DOWN+PASSABLE_UP,
};

namespace DATABASE
{
    enum SpriteType
    {
        SPRITE_TYPE_TILE,
        SPRITE_TYPE_AUTOTILE,
        SPRITE_TYPE_OBJECT,
        SPRITE_TYPE_ANIMATED_OBJECT,
        SPRITE_TYPE_TEXTBOX,
    };

    struct SpritePrototype
    {
        SpritePrototype() : m_uiSpriteType(SPRITE_TYPE_TILE), m_uiID(0), m_transparentColor(0)
        {
            memset(&Type, 0, sizeof(Type));
        }

        UINT m_uiSpriteType;
        UINT m_uiID;
        DWORD m_transparentColor;
        std::string m_sFileName;        // stores texture name (WITHOUT path).

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
                UINT m_uiBoundingXBegin;
                UINT m_uiBoundingYBegin;
                UINT m_uiBoundingXRange;
                UINT m_uiBoundingYRange;
            } Object;

            // SpriteAnimatedObject = 3
            struct SpriteAnimatedObject : SpriteObject
            {
                UINT m_uiSpritesX;
                UINT m_uiSpritesY;
            } AnimatedObject;

            // TextBox = 4
            struct TextBoxObject
            {
                UINT m_uiBorderSize;
            } Textbox;
        } Type;
    };

    enum OBJECT_TYPE
    {
        OBJECT_TYPE_MAP_OBJECT,
        OBJECT_TYPE_NPC,
    };

    struct StartConditionsPrototype
    {
        StartConditionsPrototype() : m_uiMapID(0), m_uiHeroeID(0) { }
        UINT m_uiMapID;
        UINT m_uiHeroeID;
        Point<UINT> m_uiStartPos;
    };

    struct ObjectPrototype
    {
        ObjectPrototype() : m_uiID(0), m_uiType(0), m_uiTextureID(0)
        {
            memset(&ObjectType, 0, sizeof(ObjectType));
        }

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
                UINT m_uiAnimationFrequency;
                UINT m_uiHPmin;
                UINT m_uiHPmax;
                UINT m_uiLevelMin;
                UINT m_uiLevelMax;
                UINT m_uiMoveSpeed;
            }NPC;
        }ObjectType;
    };

    typedef std::map<UINT, ObjectPrototype> ObjectList;
    typedef std::map<std::string, std::map<UINT, SpritePrototype>> SpriteList;

    class Database : public TSingleton<Database>
    {
    public:
        Database(void);
        ~Database(void);

        const std::string GetMapName(UINT p_uiID);
        bool GetStartConditions(StartConditionsPrototype &p_proto);
        const ObjectPrototype* GetObjectPrototype(UINT p_uiID);
        const std::string GetSpritePath(UINT p_uiID);

        void GetTextureNames(std::string p_sType, std::map<UINT, std::string> &p_lTextureNames);

        const SpritePrototype* GetSpritePrototype(std::string p_sType, UINT p_uiID);

        void LoadDB(std::string p_sFileName);
        XML_Reader::XML_STATE GetDBState();

    private:
        ChildList m_pDatabase;
        // objectDB
        ObjectList m_ObjectDB;
        // sprite paths
        std::map<UINT, std::string> m_SpritePaths;
        // spriteDB
        SpriteList m_SpriteDB;

        bool ChangeDBdir(std::list<std::string> p_DirList, ChildList::iterator &p_itr);
        void StoreSpritePaths();

        // load DB
        XML_Reader *m_pXMLReader;
        inline void KillXMLThread()
        {
            if (m_pXMLReader)
            {
                m_pXMLReader->Kill();
                m_pXMLReader = NULL;
            }
        }
    };
};
#endif