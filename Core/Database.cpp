#include "Database.h"
#include <fstream>
#include <atlcomcli.h>
#include "GlobalVariables.h"

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
        m_Database.clear();

        m_ObjectDB.clear();
        m_SpriteDB.clear();
        
        m_SpritePaths.clear();
    }

    const std::string Database::GetMapName(uint32 p_uiID)
    {
        std::string t_sDir[] = {"MapDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return "";

        const ReadChildList* t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return "";

        for (ReadChildList::const_iterator t_DBitr = t_pChildList->find("Map"); t_DBitr != t_pChildList->end() && t_DBitr->first == "Map"; ++t_DBitr)
        {
            if (t_DBitr->second.HasAttributes())
            {
                CComVariant t_Value;
                if (!t_DBitr->second.GetAttributeValue("ID", t_Value))
                    continue;

                if (FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;

                // check MapID
                if (t_Value.uintVal == p_uiID)
                {
                    // return MapName
                    if (!t_DBitr->second.GetAttributeValue("File", t_Value))
                        continue;

                    std::string t_temp = bstr_t(t_Value);
                    return t_temp;
                }
            }
        }
        return "";
    }

    bool Database::ChangeDBdir(std::list<std::string> p_DirList, XML_ReadData &p_Dir)
    {
        ReadChildList::iterator t_DBitr = m_Database.find("Database");
        if (t_DBitr == m_Database.end())
            return false;

        if (*p_DirList.begin() == "Database")
            p_DirList.pop_front();

        XML_ReadData *t_DBData = &t_DBitr->second;
        for (std::list<std::string>::iterator t_itr = p_DirList.begin(); t_itr != p_DirList.end(); ++t_itr)
        {
            if (!t_DBData || !(t_DBData = t_DBData->GetChild(*t_itr)))
                return false;
        }

        if (t_DBData)
        {
            p_Dir = *t_DBData;
            return true;
        }
        return false;
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
                    m_Database = *m_pXMLReader->GetXMLData();
                    StoreSpritePaths();
                    LoadGlobalVariables();
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
        else if (!m_Database.empty())
            return XML_DONE;
        else
            return XML_NONE;
    }

    bool Database::GetStartConditions(StartConditionsPrototype &p_proto)
    {
        std::string t_sDir[] = {"StartConditions"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return false;

        CComVariant t_Value;
        const AttributeList *t_AttrList = t_DBData.GetAttributeList();
        if (!t_AttrList)
            return false;

        for (AttributeList::const_iterator t_AttrItr = t_AttrList->begin(); t_AttrItr != t_AttrList->end(); ++t_AttrItr)
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

    const ObjectPrototype* Database::GetObjectPrototype(uint32 p_uiID)
    {
        ObjectList::const_iterator t_ObjItr = m_ObjectDB.find(p_uiID);
        if (t_ObjItr != m_ObjectDB.end())
            return &t_ObjItr->second;

        std::string t_sDir[] = {"ObjectDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return NULL;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return NULL;
        for (ReadChildList::const_iterator t_ObjItr = t_pChildList->find("Object"); t_ObjItr != t_pChildList->end() && t_ObjItr->first == "Object"; ++t_ObjItr)
        {
            if (t_ObjItr->second.HasAttributes())
            {
                // check ID
                CComVariant t_Value;
                if (!t_ObjItr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;

                if (t_Value.uintVal == p_uiID)
                {
                    ObjectPrototype t_Proto;
                    t_Proto.m_uiID = t_Value.uintVal;
                    // store type before other attributes
                    if (!t_ObjItr->second.GetAttributeValue("Type", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                        continue;

                    t_Proto.m_uiType = t_Value.uintVal;

                    const AttributeList *t_pAttrList = t_ObjItr->second.GetAttributeList();
                    if (!t_pAttrList)
                        continue;
                    for (AttributeList::const_iterator t_AttrItr = t_pAttrList->begin(); t_AttrItr != t_pAttrList->end(); ++t_AttrItr)
                    {
                        if (t_AttrItr->first == "ObjectName")
                            t_Proto.m_sName = bstr_t(t_AttrItr->second);
                        else if (t_AttrItr->first == "ID")
                                continue;
                        else if (t_AttrItr->first == "Type")
                            continue;

                        if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                            continue;

                        if (t_AttrItr->first == "TextureID")
                            t_Proto.m_uiTextureID = t_Value.uintVal;
                        else if (t_AttrItr->first == "AnimationFrequency")
                        {
                            switch(t_Proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_Proto.ObjectType.NPC.m_uiAnimationFrequency = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "MovementSpeed")
                        {
                            switch(t_Proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_Proto.ObjectType.NPC.m_uiMoveSpeed = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "HP_min")
                        {
                            switch(t_Proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_Proto.ObjectType.NPC.m_uiHPmin = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "HP_max")
                        {
                            switch(t_Proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_Proto.ObjectType.NPC.m_uiHPmax = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "Level_min")
                        {
                            switch(t_Proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_Proto.ObjectType.NPC.m_uiLevelMin = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "Level_max")
                        {
                            switch(t_Proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_Proto.ObjectType.NPC.m_uiLevelMax = t_Value.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    // check child list
                    const ReadChildList *t_pChildList = t_ObjItr->second.GetChildList();
                    if (!t_pChildList)
                        continue;

                    for (ReadChildList::const_iterator t_ChildItr = t_pChildList->begin(); t_ChildItr != t_pChildList->end(); ++t_ChildItr)
                    {
                        // check parents
                        if (t_ChildItr->first == "Parents")
                        {
                            const ReadChildList *t_ParentList = t_ChildItr->second.GetChildList();
                            if (!t_ParentList)
                                continue;

                            for (ReadChildList::const_iterator t_ParentItr = t_ParentList->begin(); t_ParentItr != t_ParentList->end(); ++t_ParentItr)
                            {
                                CComVariant t_IDValue;
                                if (!t_ParentItr->second.GetAttributeValue("ID", t_IDValue) || FAILED(t_IDValue.ChangeType(VT_UINT)))
                                    continue;

                                t_Proto.m_uilParentList.insert(t_IDValue.uintVal);
                            }
                        }
                        // check variables
                        else if (t_ChildItr->first == "Variables")
                        {
                            const ReadChildList *t_pVariableList = t_ChildItr->second.GetChildList();
                            if (!t_pVariableList)
                                continue;

                            for (ReadChildList::const_iterator t_VTypeItr = t_pVariableList->begin(); t_VTypeItr != t_pVariableList->end(); ++t_VTypeItr)
                            {
                                // bool value
                                if (t_VTypeItr->first == "bool")
                                {
                                    TVariable<bool> t_NewBool;
                                    const AttributeList *t_pVAttrList = t_VTypeItr->second.GetAttributeList();
                                    if (!t_pVAttrList)
                                        continue;

                                    for (AttributeList::const_iterator t_VAttrItr = t_pVAttrList->begin(); t_VAttrItr != t_pVAttrList->end(); ++t_VAttrItr)
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
                                    t_Proto.m_ObjectBoolList.insert(std::make_pair(t_NewBool.m_uiID, t_NewBool));
                                }
                                // int value
                                else if (t_VTypeItr->first == "integer")
                                {
                                    TVariable<int> t_NewInt;
                                    const AttributeList *t_pVAttrList = t_VTypeItr->second.GetAttributeList();
                                    if (!t_pVAttrList)
                                        continue;

                                    for (AttributeList::const_iterator t_VAttrItr = t_pVAttrList->begin(); t_VAttrItr != t_pVAttrList->end(); ++t_VAttrItr)
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
                                    t_Proto.m_ObjectIntegerList.insert(std::make_pair(t_NewInt.m_uiID, t_NewInt));
                                }
                                // float value
                                else if (t_VTypeItr->first == "float")
                                {
                                    TVariable<float> t_NewFloat;
                                    const AttributeList *t_pVAttrList = t_VTypeItr->second.GetAttributeList();
                                    if (!t_pVAttrList)
                                        continue;

                                    for (AttributeList::const_iterator t_VAttrItr = t_pVAttrList->begin(); t_VAttrItr != t_pVAttrList->end(); ++t_VAttrItr)
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
                                    t_Proto.m_ObjectFloatList.insert(std::make_pair(t_NewFloat.m_uiID, t_NewFloat));
                                }
                                // string value
                                else if (t_VTypeItr->first == "string")
                                {
                                    TVariable<std::string> t_NewString;
                                    const AttributeList *t_pVAttrList = t_VTypeItr->second.GetAttributeList();
                                    if (!t_pVAttrList)
                                        continue;

                                    for (AttributeList::const_iterator t_VAttrItr = t_pVAttrList->begin(); t_VAttrItr != t_pVAttrList->end(); ++t_VAttrItr)
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
                                    t_Proto.m_ObjectStringList.insert(std::make_pair(t_NewString.m_uiID, t_NewString));
                                }
                            }
                        }
                    }

                    m_ObjectDB.insert(std::make_pair(t_Proto.m_uiID, t_Proto));
                    return &(m_ObjectDB.find(p_uiID)->second);
                }
            }
        }
        return NULL;
    }

    const SpritePrototype* Database::GetSpritePrototype(std::string p_sType, uint32 p_uiID)
    {
        SpriteList::iterator t_CacheItr = m_SpriteDB.find(p_sType);
        if (t_CacheItr != m_SpriteDB.end())
        {
            std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_CacheItr->second.find(p_uiID);
            if (t_SpriteItr != t_CacheItr->second.end())
                return &t_SpriteItr->second;
        }

        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return NULL;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return NULL;

        for (ReadChildList::const_iterator t_DBitr2 = t_pChildList->find("Sprite"); t_DBitr2 != t_pChildList->end() && t_DBitr2->first == "Sprite"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                // check ID
                CComVariant t_Value;
                if (!t_DBitr2->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;

                if (t_Value.uintVal == p_uiID)
                {
                    SpritePrototype t_proto;
                    t_proto.m_uiID = t_Value.uintVal;
                    // store type before other attributes
                    if (!t_DBitr2->second.GetAttributeValue("Type", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                        continue;
                    t_proto.m_uiSpriteType = t_Value.uintVal;

                    const AttributeList *t_pAttrList = t_DBitr2->second.GetAttributeList();
                    if (!t_pAttrList)
                        continue;
                    for (AttributeList::const_iterator t_AttrItr = t_pAttrList->begin(); t_AttrItr != t_pAttrList->end(); ++t_AttrItr)
                    {
                        // check first for different variable types than uint32
                        if (t_AttrItr->first == "FileName")
                        {
                            t_proto.m_sFileName = bstr_t(t_AttrItr->second);
                            continue;
                        }
                        else if (t_AttrItr->first == "transparent_color")
                        {
                            t_proto.m_sTransparencyColor = bstr_t(t_AttrItr->second);
                            continue;
                        }

                        // check the uint32 variables
                        if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                            continue;

                        if (t_AttrItr->first == "ID")
                            continue;

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
                        std::map<uint32, SpritePrototype> t_Map;
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
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        for (ReadChildList::const_iterator t_DBitr2 = t_pChildList->find("SpritePath"); t_DBitr2 != t_pChildList->end() && t_DBitr2->first == "SpritePath"; ++t_DBitr2)
        {
            const AttributeList *t_pAttrList = t_DBitr2->second.GetAttributeList();
            if (!t_pAttrList)
                continue;

            uint32 t_uiID = 0;
            std::string t_sPath;
            for (AttributeList::const_iterator t_AttrItr = t_pAttrList->begin(); t_AttrItr != t_pAttrList->end(); ++t_AttrItr)
            {
                CComVariant t_Value;
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

    const std::string Database::GetSpritePath(uint32 p_uiID)
    {
        std::map<uint32, std::string>::iterator t_itr = m_SpritePaths.find(p_uiID);
        if (t_itr == m_SpritePaths.end())
            return "";

        return t_itr->second;
    }

    void Database::GetTextureNames(std::string p_sType, std::map<uint32, std::string> &p_lTextureNames)
    {
        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return;

        for (ReadChildList::const_iterator t_DBitr = t_pChildList->find("Sprite"); t_DBitr != t_pChildList->end() && t_DBitr->first == "Sprite"; ++t_DBitr)
        {
            if (t_DBitr->second.HasAttributes())
            {
                CComVariant t_Value;
                if (!t_DBitr->second.GetAttributeValue("FileName", t_Value))
                    continue;

                std::string t_sName = bstr_t(t_Value);
                if (!t_DBitr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;
                
                p_lTextureNames.insert(std::make_pair(t_Value.uintVal, t_sName));
            }
        }
    }

    bool Database::HasSprite(std::string p_sType, uint32 p_uiID)
    {
        SpriteList::iterator t_CacheItr = m_SpriteDB.find(p_sType);
        if (t_CacheItr != m_SpriteDB.end())
        {
            std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_CacheItr->second.find(p_uiID);
            if (t_SpriteItr != t_CacheItr->second.end())
                return true;
        }

        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return false;

        CComVariant t_Value;
        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return false;

        for (ReadChildList::const_iterator t_DBitr = t_pChildList->find("Sprite"); t_DBitr != t_pChildList->end() && t_DBitr->first == "Sprite"; ++t_DBitr)
        {
            if (!t_DBitr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                continue;

            if (t_Value.uintVal == p_uiID)
                return true;
        }
        return false;
    }

    void Database::GetObjectNames(std::map<uint32, std::string> &p_lObjectNames)
    {
        std::string t_sDir[] = {"ObjectDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return;
        for (ReadChildList::const_iterator t_DBitr = t_pChildList->find("Object"); t_DBitr != t_pChildList->end() && t_DBitr->first == "Object"; ++t_DBitr)
        {
            if (t_DBitr->second.HasAttributes())
            {
                CComVariant t_Value;
                if (!t_DBitr->second.GetAttributeValue("ObjectName", t_Value))
                    continue;
                std::string t_sName = bstr_t(t_Value);

                if (!t_DBitr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;

                p_lObjectNames.insert(std::make_pair(t_Value.uintVal, t_sName));
            }
        }
    }

    void Database::LoadBools()
    {
        std::string t_sDir[] = {"GlobalVariables", "GlobalBools"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return;
        for (ReadChildList::const_iterator t_DBitr = t_pChildList->begin(); t_DBitr != t_pChildList->end(); ++t_DBitr)
        {
            if (t_DBitr->second.HasAttributes())
            {
                TVariable<bool> t_BoolVar;
                CComVariant t_Value;
                if (!t_DBitr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;
                t_BoolVar.m_uiID = t_Value.uintVal;

                if (!t_DBitr->second.GetAttributeValue("name", t_Value))
                    continue;
                t_BoolVar.m_sName = bstr_t(t_Value);

                if (!t_DBitr->second.GetAttributeValue("value", t_Value) || FAILED(t_Value.ChangeType(VT_BOOL)))
                    continue;
                t_BoolVar.m_Value = t_Value.boolVal ? true : false;
                if (GlobalVariables *t_pGlobalVariables = GlobalVariables::Get())
                    t_pGlobalVariables->AddBool(t_BoolVar);
            }
        }
    }

    void Database::LoadIntegers()
    {
        std::string t_sDir[] = {"GlobalVariables", "GlobalInts"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return;
        for (ReadChildList::const_iterator t_DBitr = t_pChildList->begin(); t_DBitr != t_pChildList->end(); ++t_DBitr)
        {
            if (t_DBitr->second.HasAttributes())
            {
                TVariable<int> t_IntVar;
                CComVariant t_Value;
                if (!t_DBitr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;
                t_IntVar.m_uiID = t_Value.uintVal;

                if (!t_DBitr->second.GetAttributeValue("name", t_Value))
                    continue;
                t_IntVar.m_sName = bstr_t(t_Value);

                if (!t_DBitr->second.GetAttributeValue("value", t_Value) || FAILED(t_Value.ChangeType(VT_INT)))
                    continue;
                t_IntVar.m_Value = t_Value.intVal;
                if (GlobalVariables *t_pGlobalVariables = GlobalVariables::Get())
                    t_pGlobalVariables->AddInteger(t_IntVar);
            }
        }
    }

    void Database::LoadFloats()
    {
        std::string t_sDir[] = {"GlobalVariables", "GlobalFloats"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return;
        for (ReadChildList::const_iterator t_DBitr = t_pChildList->begin(); t_DBitr != t_pChildList->end(); ++t_DBitr)
        {
            if (t_DBitr->second.HasAttributes())
            {
                TVariable<float> t_FloatVar;
                CComVariant t_Value;
                if (!t_DBitr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;
                t_FloatVar.m_uiID = t_Value.uintVal;

                if (!t_DBitr->second.GetAttributeValue("name", t_Value))
                    continue;
                t_FloatVar.m_sName = bstr_t(t_Value);

                if (!t_DBitr->second.GetAttributeValue("value", t_Value) || FAILED(t_Value.ChangeType(VT_R4)))
                    continue;
                t_FloatVar.m_Value = t_Value.fltVal;
                if (GlobalVariables *t_pGlobalVariables = GlobalVariables::Get())
                    t_pGlobalVariables->AddFloat(t_FloatVar);
            }
        }
    }

    void Database::LoadStrings()
    {
        std::string t_sDir[] = {"GlobalVariables", "GlobalStrings"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!ChangeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return;
        for (ReadChildList::const_iterator t_DBitr = t_pChildList->begin(); t_DBitr != t_pChildList->end(); ++t_DBitr)
        {
            if (t_DBitr->second.HasAttributes())
            {
                TVariable<std::string> t_StringVar;
                CComVariant t_Value;
                if (!t_DBitr->second.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;
                t_StringVar.m_uiID = t_Value.uintVal;

                if (!t_DBitr->second.GetAttributeValue("name", t_Value))
                    continue;
                t_StringVar.m_sName = bstr_t(t_Value);

                if (!t_DBitr->second.GetAttributeValue("value", t_Value))
                    continue;
                t_StringVar.m_Value = bstr_t(t_Value);
                if (GlobalVariables *t_pGlobalVariables = GlobalVariables::Get())
                    t_pGlobalVariables->AddString(t_StringVar);
            }
        }
    }

    void Database::LoadGlobalVariables()
    {
        LoadBools();
        LoadIntegers();
        LoadFloats();
        LoadStrings();
    }

    std::string ObjectPrototype::GetRightTextureType(DATABASE::OBJECT_TYPE p_Type)
    {
        switch(p_Type)
        {
        case OBJECT_TYPE_NPC:
            return "NPCs";
        case OBJECT_TYPE_MAP_OBJECT:
            return "Objects";
        default:
            return "";
        }
    }

    bool Prototype::IsChildrenOf(uint32 p_uiID) const
    {
        if (!p_uiID)
            return false;

        IDList::const_iterator t_Itr = m_uilParentList.find(p_uiID);
        if (t_Itr != m_uilParentList.end())
            return true;

        return false;
    }
};