#include "Database.h"
#include <fstream>
#include <atlcomcli.h>

namespace DATABASE
{
    using namespace XML;
    Database::Database(void) : m_pXMLReader(NULL), TSingleton()
    {
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "Database : ";
    }

    Database::~Database(void)
    {
        KillXMLThread();
    }

    void Database::ClearDB()
    {
        m_pDatabase.clear();

        m_ObjectDB.clear();
        m_SpriteDB.clear();
        
        m_SpritePaths.clear();
    }

    const std::string Database::GetMapName(UINT p_uiID)
    {
        std::string t_sDir[] = {"MapDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return "";

        AttributeList::iterator t_AttrItr;
        CComVariant t_Value;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Map");
             t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Map"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                    continue;

                // check MapID
                if (t_Value.uintVal == p_uiID)
                {
                    // return MapName
                    t_AttrItr = t_DBitr2->second.m_AttributeList.find("File");
                    if (t_AttrItr != t_DBitr2->second.m_AttributeList.end())
                    {
                        std::string t_temp = bstr_t(t_AttrItr->second);
                        return t_temp;
                    }
                }
            }
        }
        return "";
    }

    bool Database::ChangeDBdir(std::list<std::string> p_DirList, ChildList::iterator &p_itr)
    {
        ChildList::iterator t_DBitr = m_pDatabase.find("Database");
        ChildList::iterator t_DBitr2;
        if (*p_DirList.begin() == "Database")
            p_DirList.pop_front();

        for (std::list<std::string>::iterator t_itr = p_DirList.begin(); t_itr != p_DirList.end(); ++t_itr)
        {
            t_DBitr2 = t_DBitr->second.m_ChildList.find(*t_itr);
            if (t_DBitr2 == t_DBitr->second.m_ChildList.end())
                return false;

            t_DBitr = t_DBitr2;
        }

        p_itr = t_DBitr;
        return true;
    }

    void Database::LoadDB(std::string p_sFileName)
    {
        ClearDB();
        // start XML Reader
        std::string t_sFileData;
        // this open and store the XML file:
        std::fstream Data(p_sFileName.c_str());
        if(Data)
        {
            std::string DataLine;
            while(std::getline(Data,DataLine))
                t_sFileData += DataLine;

            m_pXMLReader = new XML_Reader(t_sFileData);
            Data.close();
        }
    }

    XML_STATE Database::GetDBState()
    {
        // progress XML Reader
        if (m_pXMLReader)
        {
            XML_STATE t_State = m_pXMLReader->GetReaderState();
            switch(t_State)
            {
            case XML_IN_PROGRESS:
                return XML_IN_PROGRESS;
            case XML_DONE:
                if (m_pXMLReader->GetXMLData())
                {
                    m_pDatabase = *m_pXMLReader->GetXMLData();
                    StoreSpritePaths();
                    BASIC_LOG(m_sLogLocationName + "Database load complete.");
                }
                else
                    ERROR_LOG(m_sLogLocationName + "Database load complete, but got no Data from file.");
                break;
            default:
                break;
            }

            KillXMLThread();
            return t_State;
        }
        else if (!m_pDatabase.empty())
            return XML_DONE;
        else
            return XML_NONE;
    }

    bool Database::GetStartConditions(StartConditionsPrototype &p_proto)
    {
        std::string t_sDir[] = {"StartConditions"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return false;

        CComVariant t_Value;
        for (AttributeList::iterator t_AttrItr = t_DBitr->second.m_AttributeList.begin();
             t_AttrItr != t_DBitr->second.m_AttributeList.end(); ++t_AttrItr)
        {
            if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                continue;

            // MapID
            if (t_AttrItr->first == "MapID")
                p_proto.m_uiMapID = t_Value.uintVal;

            // HeroID
            else if (t_AttrItr->first == "HeroeID")
                p_proto.m_uiHeroID = t_Value.uintVal;

            // X starting pos
            else if (t_AttrItr->first == "StartPosX")
                p_proto.m_uiStartPos.x = t_Value.uintVal;

            // Y starting pos
            else if (t_AttrItr->first == "StartPosY")
                p_proto.m_uiStartPos.y = t_Value.uintVal;
        }
        return true;
    }

    const ObjectPrototype* Database::GetObjectPrototype(UINT p_uiID)
    {
        ObjectList::const_iterator t_ObjItr = m_ObjectDB.find(p_uiID);
        if (t_ObjItr != m_ObjectDB.end())
            return &t_ObjItr->second;

        std::string t_sDir[] = {"ObjectDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return NULL;

        AttributeList::iterator t_AttrItr;
        ObjectPrototype t_proto;
        CComVariant t_Value;
        for (ChildList::iterator t_ObjItr = t_DBitr->second.m_ChildList.find("Object");
             t_ObjItr != t_DBitr->second.m_ChildList.end() && t_ObjItr->first == "Object"; ++t_ObjItr)
        {
            if (t_ObjItr->second.HasAttributes())
            {
                t_AttrItr = t_ObjItr->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_ObjItr->second.m_AttributeList.end())
                    continue;

                // check ID
                if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                    continue;
                if (t_Value.uintVal == p_uiID)
                {
                    t_proto.m_uiID = t_Value.uintVal;
                    // store type before other
                    t_AttrItr = t_ObjItr->second.m_AttributeList.find("Type");
                    if (t_AttrItr == t_ObjItr->second.m_AttributeList.end())
                        return NULL;

                    if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                        continue;
                    t_proto.m_uiType = t_Value.uiVal;

                    for (t_AttrItr = t_ObjItr->second.m_AttributeList.begin();
                    t_AttrItr != t_ObjItr->second.m_AttributeList.end(); ++t_AttrItr)
                    {
                        if (t_AttrItr->first == "ObjectName")
                            t_proto.m_sName = bstr_t(t_AttrItr->second);
                        else if (t_AttrItr->first == "ID")
                                continue;
                        else if (t_AttrItr->first == "Type")
                            continue;

                        if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                            continue;

                        if (t_AttrItr->first == "TextureID")
                            t_proto.m_uiTextureID = t_Value.intVal;
                        else if (t_AttrItr->first == "AnimationFrequency")
                        {
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiAnimationFrequency = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "MovementSpeed")
                        {
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiMoveSpeed = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "HP_min")
                        {
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiHPmin = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "HP_max")
                        {
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiHPmax = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "Level_min")
                        {
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiLevelMin = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "Level_max")
                        {
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiLevelMax = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    // check child list
                    for (ChildList::iterator t_ChildItr = t_ObjItr->second.m_ChildList.begin(); t_ChildItr != t_ObjItr->second.m_ChildList.end(); ++t_ChildItr)
                    {
                        // check variables
                        if (t_ChildItr->first == "Variables")
                        {
                            for (ChildList::iterator t_VTypeItr = t_ChildItr->second.m_ChildList.begin(); t_VTypeItr != t_ChildItr->second.m_ChildList.end(); ++t_VTypeItr)
                            {
                                // bool value
                                if (t_VTypeItr->first == "bool")
                                {
                                    TObjectVariable<bool> t_NewBool;
                                    for (AttributeList::iterator t_VAttrItr = t_VTypeItr->second.m_AttributeList.begin(); t_VAttrItr != t_VTypeItr->second.m_AttributeList.end(); ++t_VAttrItr)
                                    {
                                        if (t_VAttrItr->first == "name")
                                            t_NewBool.m_sName = bstr_t(t_VAttrItr->second);
                                        else if (t_VAttrItr->first == "ID")
                                        {
                                            if (FAILED(t_Value.ChangeType(VT_UINT, &t_VAttrItr->second)))
                                                continue;

                                            t_NewBool.m_uiID = t_Value.uintVal;
                                        }
                                        else if (t_VAttrItr->first == "value")
                                        {
                                            if (FAILED(t_Value.ChangeType(VT_BOOL, &t_VAttrItr->second)))
                                                continue;

                                            t_NewBool.m_Value = t_Value.boolVal == VARIANT_FALSE ? false : true;
                                        }
                                    }
                                    t_proto.m_ObjectBoolList.insert(std::make_pair(t_NewBool.m_uiID, t_NewBool));
                                }
                                // int value
                                else if (t_VTypeItr->first == "integer")
                                {
                                    TObjectVariable<int> t_NewInt;
                                    for (AttributeList::iterator t_VAttrItr = t_VTypeItr->second.m_AttributeList.begin(); t_VAttrItr != t_VTypeItr->second.m_AttributeList.end(); ++t_VAttrItr)
                                    {
                                        if (t_VAttrItr->first == "name")
                                            t_NewInt.m_sName = bstr_t(t_VAttrItr->second);
                                        else if (t_VAttrItr->first == "ID")
                                        {
                                            if (FAILED(t_Value.ChangeType(VT_UINT, &t_VAttrItr->second)))
                                                continue;

                                            t_NewInt.m_uiID = t_Value.uintVal;
                                        }
                                        else if (t_VAttrItr->first == "value")
                                        {
                                            if (FAILED(t_Value.ChangeType(VT_INT, &t_VAttrItr->second)))
                                                continue;

                                            t_NewInt.m_Value = t_Value.intVal;
                                        }
                                    }
                                    t_proto.m_ObjectIntegerList.insert(std::make_pair(t_NewInt.m_uiID, t_NewInt));
                                }
                                // float value
                                else if (t_VTypeItr->first == "float")
                                {
                                    TObjectVariable<float> t_NewFloat;
                                    for (AttributeList::iterator t_VAttrItr = t_VTypeItr->second.m_AttributeList.begin(); t_VAttrItr != t_VTypeItr->second.m_AttributeList.end(); ++t_VAttrItr)
                                    {
                                        if (t_VAttrItr->first == "name")
                                            t_NewFloat.m_sName = bstr_t(t_VAttrItr->second);
                                        else if (t_VAttrItr->first == "ID")
                                        {
                                            if (FAILED(t_Value.ChangeType(VT_UINT, &t_VAttrItr->second)))
                                                continue;

                                            t_NewFloat.m_uiID = t_Value.uintVal;
                                        }
                                        else if (t_VAttrItr->first == "value")
                                        {
                                            if (FAILED(t_Value.ChangeType(VT_R4, &t_VAttrItr->second)))
                                                continue;

                                            t_NewFloat.m_Value = t_Value.fltVal;
                                        }
                                    }
                                    t_proto.m_ObjectFloatList.insert(std::make_pair(t_NewFloat.m_uiID, t_NewFloat));
                                }
                                // string value
                                else if (t_VTypeItr->first == "string")
                                {
                                    TObjectVariable<std::string> t_NewString;
                                    for (AttributeList::iterator t_VAttrItr = t_VTypeItr->second.m_AttributeList.begin(); t_VAttrItr != t_VTypeItr->second.m_AttributeList.end(); ++t_VAttrItr)
                                    {
                                        if (t_VAttrItr->first == "name")
                                            t_NewString.m_sName = bstr_t(t_VAttrItr->second);
                                        else if (t_VAttrItr->first == "ID")
                                        {
                                            if (FAILED(t_Value.ChangeType(VT_UINT, &t_VAttrItr->second)))
                                                continue;

                                            t_NewString.m_uiID = t_Value.uintVal;
                                        }
                                        else if (t_VAttrItr->first == "value")
                                            t_NewString.m_Value = bstr_t(t_VAttrItr->second);
                                    }
                                    t_proto.m_ObjectStringList.insert(std::make_pair(t_NewString.m_uiID, t_NewString));
                                }
                            }
                        }
                    }

                    m_ObjectDB.insert(std::make_pair(t_proto.m_uiID, t_proto));
                    return &(m_ObjectDB.find(p_uiID)->second);
                }
            }
        }
        return NULL;
    }

    const SpritePrototype* Database::GetSpritePrototype(std::string p_sType, UINT p_uiID)
    {
        SpriteList::iterator t_CacheItr = m_SpriteDB.find(p_sType);
        if (t_CacheItr != m_SpriteDB.end())
        {
            std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_CacheItr->second.find(p_uiID);
            if (t_SpriteItr != t_CacheItr->second.end())
                return &t_SpriteItr->second;
        }

        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return NULL;

        AttributeList::iterator t_AttrItr;
        SpritePrototype t_proto;
        CComVariant t_Value;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Sprite");
             t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Sprite"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                // check ID
                if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                    continue;

                if (t_Value.uintVal == p_uiID)
                {
                    // store type before other
                    t_AttrItr = t_DBitr2->second.m_AttributeList.find("Type");
                    if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                        return NULL;

                    if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                        continue;
                    t_proto.m_uiSpriteType = t_Value.uintVal;

                    for (t_AttrItr = t_DBitr2->second.m_AttributeList.begin();
                    t_AttrItr != t_DBitr2->second.m_AttributeList.end(); ++t_AttrItr)
                    {
                        // check first for different variable types than UINT
                        if (t_AttrItr->first == "FileName")
                        {
                            t_proto.m_sFileName = bstr_t(t_AttrItr->second);
                            continue;
                        }
                        else if (t_AttrItr->first == "transparent_color")
                        {
                            t_proto.m_sTransparentColor = bstr_t(t_AttrItr->second);
                            continue;
                        }

                        // check the UINT variables
                        if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                            continue;

                        if (t_AttrItr->first == "ID")
                            t_proto.m_uiID = t_Value.uintVal;

                        else if (t_AttrItr->first == "Type")
                            continue;

                        else if (t_AttrItr->first == "spritesX")
                        {
                            switch(t_proto.m_uiSpriteType)
                            {
                                // count of spritesX
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.AnimatedObject.m_uiSpritesX = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "spritesY")
                        {
                            switch(t_proto.m_uiSpriteType)
                            {
                                // count of spritesX
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.AnimatedObject.m_uiSpritesY = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingXBegin")
                        {
                            if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                                continue;
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingXBegin = t_Value.intVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingYBegin")
                        {
                            if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                                continue;
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingYBegin = t_Value.intVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingXRange")
                        {
                            if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                                continue;
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingXRange = t_Value.intVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingYRange")
                        {
                            if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                                continue;
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingYRange = t_Value.intVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "passability")
                        {
                            switch(t_proto.m_uiSpriteType)
                            {
                                // passability
                            case SPRITE_TYPE_TILE:
                                t_proto.Type.Tile.m_uiPassable = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "terraintype")
                        {
                            switch(t_proto.m_uiSpriteType)
                            {
                                // terrain type
                            case SPRITE_TYPE_TILE:
                                t_proto.Type.Tile.m_uiTerrainType = t_Value.uintVal;
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "auto_tile")
                        {
                            switch(t_proto.m_uiSpriteType)
                            {
                                // auto_tile
                            case SPRITE_TYPE_TILE:
                                t_proto.Type.Tile.m_bAutotile = t_Value.uintVal != 0 ? true : false;
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "BorderSize")
                        {
                            switch(t_proto.m_uiSpriteType)
                            {
                                // border size
                            case SPRITE_TYPE_TEXTBOX:
                                t_proto.Type.Textbox.m_uiBorderSize = t_Value.uintVal;
                                break;
                            }
                        }
                    }

                    if (m_SpriteDB.find(p_sType) == m_SpriteDB.end())
                    {
                        std::map<UINT, SpritePrototype> t_Map;
                        m_SpriteDB.insert(std::make_pair(p_sType, t_Map));
                    }

                    m_SpriteDB.find(p_sType)->second.insert(std::make_pair(t_proto.m_uiID, t_proto));
                    return &(m_SpriteDB.find(p_sType)->second.find(p_uiID)->second);
                }
            }
        }
        return NULL;
    }

    void Database::StoreSpritePaths()
    {
        m_SpritePaths.clear();
        std::string t_sDir[] = {"SpriteDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return;

        AttributeList::iterator t_AttrItr;
        UINT t_uiID = 0;
        std::string t_sPath;
        CComVariant t_Value;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("SpritePath");
             t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "SpritePath"; ++t_DBitr2)
        {
            for (t_AttrItr = t_DBitr2->second.m_AttributeList.begin();
            t_AttrItr != t_DBitr2->second.m_AttributeList.end(); ++t_AttrItr)
            {
                if (t_AttrItr->first == "ID")
                {
                    if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                        continue;

                    t_uiID = t_Value.uintVal;
                }
                else if (t_AttrItr->first == "Path")
                    t_sPath = bstr_t(t_AttrItr->second);
            }

            m_SpritePaths.insert(std::make_pair(t_uiID, t_sPath));
        }
    }

    const std::string Database::GetSpritePath(UINT p_uiID)
    {
        std::map<UINT, std::string>::iterator t_itr = m_SpritePaths.find(p_uiID);
        if (t_itr == m_SpritePaths.end())
            return "";

        return t_itr->second;
    }

    void Database::GetTextureNames(std::string p_sType, std::map<UINT, std::string> &p_lTextureNames)
    {
        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return;

        std::string t_sName;
        AttributeList::iterator t_AttrItr;
        CComVariant t_Value;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Sprite");
            t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Sprite"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                t_AttrItr = t_DBitr2->second.m_AttributeList.find("FileName");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                t_sName = bstr_t(t_AttrItr->second);

                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                    continue;
                
                p_lTextureNames.insert(std::make_pair(t_Value.uintVal, t_sName));
            }
        }
    }

    bool Database::HasSprite(std::string p_sType, UINT p_uiID)
    {
        SpriteList::iterator t_CacheItr = m_SpriteDB.find(p_sType);
        if (t_CacheItr != m_SpriteDB.end())
        {
            std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_CacheItr->second.find(p_uiID);
            if (t_SpriteItr != t_CacheItr->second.end())
                return true;
        }

        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return true;

        CComVariant t_Value;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Sprite");
            t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Sprite"; ++t_DBitr2)
        {
            if (!t_DBitr2->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                continue;

            if (t_Value.uintVal == p_uiID)
                return true;
        }
        return false;
    }

    void Database::GetObjectNames(std::map<UINT, std::string> &p_lObjectNames)
    {
        std::string t_sDir[] = {"ObjectDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return;

        std::string t_sName;
        AttributeList::iterator t_AttrItr;
        CComVariant t_Value;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Object");
            t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Object"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ObjectName");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                t_sName = bstr_t(t_AttrItr->second);

                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                    continue;

                p_lObjectNames.insert(std::make_pair(t_Value.uintVal, t_sName));
            }
        }
    }
};