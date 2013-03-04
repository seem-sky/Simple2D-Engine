#ifndef DATABASE_H
#define DATABASE_H

#include "XML_Reader.h"
#include "Singleton.h"
#include <list>
#include "Point.h"
#include "Logfile.h"
#include "VariableHolder.h"
#include "EventScriptHolder.h"
#include <set>
#include "Color.h"

class DatabaseLoad;

typedef std::set<uint32> IDList;

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
    /*#####
    # Prototype Superclass
    #####*/
    enum PrototypeType
    {
        PROTOTYPE_TYPE_NONE,
        PROTOTYPE_TYPE_OBJECT,
    };

    struct Prototype
    {
        Prototype() : m_uiID(0), m_PrototypeType(PROTOTYPE_TYPE_NONE) {}
        bool IsChildrenOf(uint32 p_uiID) const;
        inline const IDList* GetParentList() const { return &m_uilParentList; }

        uint32 m_uiID;
        std::string m_sName;
        IDList m_uilParentList;
        PrototypeType m_PrototypeType;
    };

    /*####
    # SpritePrototype
    ####*/
    enum SpriteType
    {
        SPRITE_TYPE_TILE,
        SPRITE_TYPE_OBJECT,
        SPRITE_TYPE_ANIMATED_OBJECT,
        SPRITE_TYPE_TEXTBOX,
    };

    struct SpritePrototype
    {
        SpritePrototype() : m_uiSpriteType(SPRITE_TYPE_TILE), m_uiID(0)
        {
            memset(&Type, 0, sizeof(Type));
        }

        uint32 m_uiSpriteType;
        uint32 m_uiID;
        Color m_TransparencyColor;
        std::string m_sFileName;        // stores texture name (WITHOUT path).
        std::string m_sPath;            // path from ~/textures

        std::string GetFilePath() const { return m_sPath + m_sFileName; }

        union                           // different sprite types have different attributes
        {
            // SpriteTile = 0
            struct SpriteTile
            {
                uint32 m_uiPassable;        // 0 passability
                uint32 m_uiTerrainType;     // 1 terrain type
                bool m_bAutotile;           // 2 is autotile?
            } Tile;

            // SpriteObject = 1
            struct SpriteObject
            {
                int32 m_uiBoundingXBegin;
                int32 m_uiBoundingYBegin;
                int32 m_uiBoundingXRange;
                int32 m_uiBoundingYRange;
            } Object;

            // SpriteAnimatedObject = 2
            struct SpriteAnimatedObject : SpriteObject
            {
                uint32 m_uiSpritesX;
                uint32 m_uiSpritesY;
            } AnimatedObject;

            // TextBox = 3
            struct TextBoxObject
            {
                uint32 m_uiBorderSize;
            } Textbox;
        } Type;
    };

    /*####
    # StartConditionsPrototype
    ####*/
    struct StartConditionsPrototype
    {
        StartConditionsPrototype() : m_uiMapID(0), m_uiHeroID(0) { }
        uint32 m_uiMapID;
        uint32 m_uiHeroID;
        Point<uint32> m_uiStartPos;
    };

    /*####
    # ObjectPrototype
    ####*/
    enum OBJECT_TYPE
    {
        OBJECT_TYPE_MAP_OBJECT,
        OBJECT_TYPE_NPC,
    };

    struct ObjectPrototype : public Prototype
    {
        ObjectPrototype() : m_uiType(0), m_uiTextureID(0)
        {
            memset(&ObjectType, 0, sizeof(ObjectType));
            m_PrototypeType = PROTOTYPE_TYPE_OBJECT;
        }

        std::string GetCorrectTextureType() const
        {
            return GetCorrectTextureType((OBJECT_TYPE)m_uiType);
        }

        static std::string GetCorrectTextureType(OBJECT_TYPE p_Type);
        const VariableHolder* GetVariableHolderFromParent(uint32 p_uiID) const;

        uint32 m_uiType;
        uint32 m_uiTextureID;
        VariableHolder m_Variables;
        EVENT_SCRIPT::EventScriptHolder m_Scripts;

        union
        {
            // 0 = MapObject
            struct sMapObject
            {
            }MapObject;

            // 1 = NPC
            struct sNPC : sMapObject
            {
                uint32 m_uiAnimationFrequency;
                uint32 m_uiHPmin;
                uint32 m_uiHPmax;
                uint32 m_uiLevelMin;
                uint32 m_uiLevelMax;
                uint32 m_uiMoveSpeed;
            }NPC;
        }ObjectType;
    };

    typedef std::map<uint32, ObjectPrototype> ObjectPrototypeMap;
    typedef std::map<uint32, SpritePrototype> SpritePrototypeMap;
    typedef std::map<std::string, SpritePrototypeMap> SpriteTypeList;

    class Database : public TSingleton<Database>
    {
        friend class DatabaseLoad;
    public:
        Database(void);

        const std::string GetMapName(uint32 p_uiID);
        bool GetStartConditions(StartConditionsPrototype &p_proto);

        void GetObjectNames(std::map<uint32, std::string> &p_lObjectNames);
        const ObjectPrototype* GetObjectPrototype(uint32 p_uiID);

        const SpritePrototypeMap* GetTexturePrototypes(std::string p_sType) const;
        void GetTextureNames(std::string p_sType, std::map<uint32, std::string> &p_lTextureNames) const;
        const SpritePrototype* GetSpritePrototype(std::string p_sType, uint32 p_uiID);
        bool HasSprite(std::string p_sType, uint32 p_uiID);

        void LoadGlobalVariables();
        const VariableHolder* GetGlobalVariables() const { return &m_GlobalVariables; }

        void LoadDB(std::string p_sFileName);
        ThreadState GetDBState();
        bool IsDBEmpty() { return m_Database.empty(); }

    private:
        XML::ReadChildList m_Database;
        // objectDB
        ObjectPrototypeMap m_ObjectDB;
        // spriteDB
        SpriteTypeList m_SpriteDB;
        // global variables
        VariableHolder m_GlobalVariables;

        void _clearDB();
        void _storeTextures(bool &result);
        bool _loadTextureFromXML(const XML::XML_ReadData &p_Data, SpritePrototype &p_Result);

        bool _changeDBdir(std::list<std::string> p_DirList, XML::XML_ReadData &p_Dir) const;
    };
};
#endif