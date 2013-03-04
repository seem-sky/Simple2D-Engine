#include "Database.h"
#include <boost/thread.hpp>
#include <fstream>
#include <atlcomcli.h>
#include "VariableHolder.h"
#include "DatabaseLoad.h"

namespace DATABASE
{
    using namespace XML;
    Database::Database(void) : TSingleton()
    {
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "Database : ";
    }

    void Database::_clearDB()
    {
        m_Database.clear();

        m_ObjectDB.clear();
        m_SpriteDB.clear();
        m_GlobalVariables.Clear();
    }

    const std::string Database::GetMapName(uint32 p_uiID)
    {
        std::string t_sDir[] = {"MapDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!_changeDBdir(t_DirList, t_DBData))
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

    bool Database::_changeDBdir(std::list<std::string> p_DirList, XML_ReadData &p_Dir) const
    {
        ReadChildList::const_iterator t_DBitr = m_Database.find("Database");
        if (t_DBitr == m_Database.end())
            return false;

        if (*p_DirList.begin() == "Database")
            p_DirList.pop_front();

        const XML_ReadData *t_DBData = &t_DBitr->second;
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
        _clearDB();
        XML_Reader reader(p_sFileName);
        if (reader.ReadFile())
        {
            m_Database = reader.GetXMLData();
            bool spriteResult = false;
            boost::thread storeTextures(boost::bind(&Database::_storeTextures, this, spriteResult));
            LoadGlobalVariables();
            storeTextures.join();
            if (!spriteResult)
                return;

            BASIC_LOG(m_sLogLocationName + "Database load complete.");
        }
    }

    ThreadState Database::GetDBState()
    {
        //// progress XML Reader
        //if (m_pXMLReader)
        //{
        //    ThreadState t_State = m_pXMLReader->GetThreadState();
        //    switch(t_State)
        //    {
        //    case THREAD_IN_PROGRESS:
        //        return THREAD_IN_PROGRESS;
        //    case THREAD_DONE:
        //        if (m_pXMLReader->GetXMLData())
        //        {
        //            m_Database = *m_pXMLReader->GetXMLData();
        //            StoreSpritePaths();
        //            StoreTextures();
        //            LoadGlobalVariables();
        //            BASIC_LOG(m_sLogLocationName + "Database load complete.");
        //        }
        //        else
        //            ERROR_LOG(m_sLogLocationName + "Database load complete, but got no Data from file.");
        //        break;
        //    default:
        //        break;
        //    }

        //    KillXMLThread();
        //    return t_State;
        //}
        //else if (!m_Database.empty())
        //    return THREAD_DONE;
        //else
        //    return THREAD_NONE;

        return THREAD_DONE;
    }

    bool Database::GetStartConditions(StartConditionsPrototype &p_proto)
    {
        std::string t_sDir[] = {"StartConditions"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!_changeDBdir(t_DirList, t_DBData))
            return false;

        CComVariant t_Value;
        const AttributeMap *t_AttrList = t_DBData.GetAttributeList();
        if (!t_AttrList)
            return false;

        for (AttributeMap::const_iterator t_AttrItr = t_AttrList->begin(); t_AttrItr != t_AttrList->end(); ++t_AttrItr)
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
        ObjectPrototypeMap::const_iterator t_ObjItr = m_ObjectDB.find(p_uiID);
        if (t_ObjItr != m_ObjectDB.end())
            return &t_ObjItr->second;

        std::string t_sDir[] = {"ObjectDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!_changeDBdir(t_DirList, t_DBData))
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

                    const AttributeMap *t_pAttrList = t_ObjItr->second.GetAttributeList();
                    if (!t_pAttrList)
                        continue;
                    for (AttributeMap::const_iterator t_AttrItr = t_pAttrList->begin(); t_AttrItr != t_pAttrList->end(); ++t_AttrItr)
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
                            t_Proto.m_Variables.LoadDataFromXML(t_ChildItr->second);
                        // get scripts
                        else if (t_ChildItr->first == "Scripts")
                            t_Proto.m_Scripts.LoadDataFromXML(t_ChildItr->second);
                    }

                    m_ObjectDB.insert(std::make_pair(t_Proto.m_uiID, t_Proto));
                    return &(m_ObjectDB.find(p_uiID)->second);
                }
            }
        }
        return NULL;
    }

    const SpritePrototypeMap* Database::GetTexturePrototypes(std::string p_sType) const
    {
        SpriteTypeList::const_iterator t_CacheItr = m_SpriteDB.find(p_sType);
        if (t_CacheItr != m_SpriteDB.end())
            return &t_CacheItr->second;

        return NULL;
    }

    bool Database::_loadTextureFromXML(const XML::XML_ReadData &p_Data, SpritePrototype &p_Result)
    {
        const AttributeMap *t_pAttrList = p_Data.GetAttributeList();
        if (!t_pAttrList)
            return false;

        // check ID
        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT)))
            return false;

        p_Result.m_uiID = t_Value.uintVal;
        for (AttributeMap::const_iterator t_AttrItr = t_pAttrList->begin(); t_AttrItr != t_pAttrList->end(); ++t_AttrItr)
        {
            // check first for different variable types than uint32
            if (t_AttrItr->first == "FileName")
            {
                p_Result.m_sFileName = bstr_t(t_AttrItr->second);
                continue;
            }
            else if (t_AttrItr->first == "Path")
            {
                p_Result.m_sPath = bstr_t(t_AttrItr->second);
                continue;
            }
            else if (t_AttrItr->first == "transparent_color")
            {
                std::string t_sColorString = bstr_t(t_AttrItr->second);
                p_Result.m_TransparencyColor.setColor(t_sColorString);
                continue;
            }

            // check the uint32 variables
            if (FAILED(t_Value.ChangeType(VT_UINT, &t_AttrItr->second)))
                continue;

            if (t_AttrItr->first == "ID")
                continue;
            else if (t_AttrItr->first == "Type")
            {
                p_Result.m_uiSpriteType = t_Value.uintVal;
                continue;
            }
            else if (t_AttrItr->first == "spritesX")
            {
                switch(p_Result.m_uiSpriteType)
                {
                    // count of spritesX
                case SPRITE_TYPE_ANIMATED_OBJECT:
                    p_Result.Type.AnimatedObject.m_uiSpritesX = t_Value.uintVal;
                    break;
                default:
                    break;
                }
            }
            else if (t_AttrItr->first == "spritesY")
            {
                switch(p_Result.m_uiSpriteType)
                {
                    // count of spritesX
                case SPRITE_TYPE_ANIMATED_OBJECT:
                    p_Result.Type.AnimatedObject.m_uiSpritesY = t_Value.uintVal;
                    break;
                default:
                    break;
                }
            }
            else if (t_AttrItr->first == "boundingXBegin")
            {
                if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                    continue;
                switch(p_Result.m_uiSpriteType)
                {
                case SPRITE_TYPE_OBJECT:
                case SPRITE_TYPE_ANIMATED_OBJECT:
                    p_Result.Type.Object.m_uiBoundingXBegin = t_Value.intVal;
                    break;
                default:
                    break;
                }
            }
            else if (t_AttrItr->first == "boundingYBegin")
            {
                if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                    continue;
                switch(p_Result.m_uiSpriteType)
                {
                case SPRITE_TYPE_OBJECT:
                case SPRITE_TYPE_ANIMATED_OBJECT:
                    p_Result.Type.Object.m_uiBoundingYBegin = t_Value.intVal;
                    break;
                default:
                    break;
                }
            }
            else if (t_AttrItr->first == "boundingXRange")
            {
                if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                    continue;
                switch(p_Result.m_uiSpriteType)
                {
                case SPRITE_TYPE_OBJECT:
                case SPRITE_TYPE_ANIMATED_OBJECT:
                    p_Result.Type.Object.m_uiBoundingXRange = t_Value.intVal;
                    break;
                default:
                    break;
                }
            }
            else if (t_AttrItr->first == "boundingYRange")
            {
                if (FAILED(t_Value.ChangeType(VT_INT, &t_AttrItr->second)))
                    continue;
                switch(p_Result.m_uiSpriteType)
                {
                case SPRITE_TYPE_OBJECT:
                case SPRITE_TYPE_ANIMATED_OBJECT:
                    p_Result.Type.Object.m_uiBoundingYRange = t_Value.intVal;
                    break;
                default:
                    break;
                }
            }
            else if (t_AttrItr->first == "passability")
            {
                switch(p_Result.m_uiSpriteType)
                {
                    // passability
                case SPRITE_TYPE_TILE:
                    p_Result.Type.Tile.m_uiPassable = t_Value.uintVal;
                    break;
                default:
                    break;
                }
            }
            else if (t_AttrItr->first == "terraintype")
            {
                switch(p_Result.m_uiSpriteType)
                {
                    // terrain type
                case SPRITE_TYPE_TILE:
                    p_Result.Type.Tile.m_uiTerrainType = t_Value.uintVal;
                    break;
                }
            }
            else if (t_AttrItr->first == "auto_tile")
            {
                switch(p_Result.m_uiSpriteType)
                {
                    // auto_tile
                case SPRITE_TYPE_TILE:
                    p_Result.Type.Tile.m_bAutotile = t_Value.uintVal != 0 ? true : false;
                    break;
                }
            }
            else if (t_AttrItr->first == "BorderSize")
            {
                switch(p_Result.m_uiSpriteType)
                {
                    // border size
                case SPRITE_TYPE_TEXTBOX:
                    p_Result.Type.Textbox.m_uiBorderSize = t_Value.uintVal;
                    break;
                }
            }
        }
        return true;
    }

    void Database::_storeTextures(bool &result)
    {
        result = false;
        std::string t_sDir[] = {"SpriteDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!_changeDBdir(t_DirList, t_DBData))
            return;
        const ReadChildList *t_pTypeList = t_DBData.GetChildList();
        if (!t_pTypeList)
            return;

        for (ReadChildList::const_iterator t_TypeItr = t_pTypeList->begin(); t_TypeItr != t_pTypeList->end(); ++t_TypeItr)
        {
            const ReadChildList *t_pSpriteList = t_TypeItr->second.GetChildList();
            if (!t_pSpriteList)
                return;

            SpritePrototypeMap t_SpriteStoreList;
            for (ReadChildList::const_iterator t_SpriteItr = t_pSpriteList->begin(); t_SpriteItr != t_pSpriteList->end(); ++t_SpriteItr)
            {
                if (t_SpriteItr->first != "Sprite")
                    continue;

                SpritePrototype t_Proto;
                if (_loadTextureFromXML(t_SpriteItr->second, t_Proto))
                    t_SpriteStoreList.insert(std::make_pair(t_Proto.m_uiID, t_Proto));
            }

            m_SpriteDB.insert(std::make_pair(t_TypeItr->first, t_SpriteStoreList));
        }

        result = true;
    }

    const SpritePrototype* Database::GetSpritePrototype(std::string p_sType, uint32 p_uiID)
    {
        SpriteTypeList::iterator t_CacheItr = m_SpriteDB.find(p_sType);
        if (t_CacheItr != m_SpriteDB.end())
        {
            std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_CacheItr->second.find(p_uiID);
            if (t_SpriteItr != t_CacheItr->second.end())
                return &t_SpriteItr->second;
        }

        return NULL;
    }

    void Database::GetTextureNames(std::string p_sType, std::map<uint32, std::string> &p_lTextureNames) const
    {
        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!_changeDBdir(t_DirList, t_DBData))
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
        SpriteTypeList::iterator t_CacheItr = m_SpriteDB.find(p_sType);
        if (t_CacheItr != m_SpriteDB.end())
        {
            std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_CacheItr->second.find(p_uiID);
            if (t_SpriteItr != t_CacheItr->second.end())
                return true;
        }

        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!_changeDBdir(t_DirList, t_DBData))
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
        if (!_changeDBdir(t_DirList, t_DBData))
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

    void Database::LoadGlobalVariables()
    {
        std::string t_sDir[] = {"GlobalVariables"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        XML_ReadData t_DBData;
        if (!_changeDBdir(t_DirList, t_DBData))
            return;

        const ReadChildList *t_pChildList = t_DBData.GetChildList();
        if (!t_pChildList)
            return;
        for (ReadChildList::const_iterator t_DBitr = t_pChildList->begin(); t_DBitr != t_pChildList->end(); ++t_DBitr)
        {
            if (t_DBitr->first == "bool")
                m_GlobalVariables.AddBoolFromXML(t_DBitr->second);
            else if (t_DBitr->first == "integer")
                m_GlobalVariables.AddIntegerFromXML(t_DBitr->second);
            else if (t_DBitr->first == "float")
                m_GlobalVariables.AddFloatFromXML(t_DBitr->second);
            else if (t_DBitr->first == "string")
                m_GlobalVariables.AddStringFromXML(t_DBitr->second);
        }
    }

    std::string ObjectPrototype::GetCorrectTextureType(DATABASE::OBJECT_TYPE p_Type)
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