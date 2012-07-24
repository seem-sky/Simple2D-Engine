#include "Database.h"
#include <fstream>

namespace DATABASE
{
    Database::Database(void) : m_pXMLReader(NULL), TSingleton()
    {
    }

    Database::~Database(void)
    {
        KillXMLThread();
    }

    const std::string Database::GetMapName(UINT p_uiID)
    {
        std::string t_sDir[] = {"MapDatabase"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return "";

        AttributeList::iterator t_AttrItr;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Map");
             t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Map"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                // check MapID
                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                if (t_AttrItr->second.uintVal == p_uiID)
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
        m_ObjectDB.clear();
        m_SpriteDB.clear();
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

    XML_Reader::XML_STATE Database::GetDBState()
    {
        // progress XML Reader
        if (m_pXMLReader)
        {
            XML_Reader::XML_STATE t_State = m_pXMLReader->GetReaderState();
            switch(t_State)
            {
            case XML_Reader::XML_IN_PROGRESS:
                return XML_Reader::XML_IN_PROGRESS;
            case XML_Reader::XML_DONE:
                m_pDatabase = *m_pXMLReader->GetXMLData();
                StoreSpritePaths();
                break;
            default:
                break;
            }

            KillXMLThread();
            return t_State;
        }
        else if (!m_pDatabase.empty())
            return XML_Reader::XML_DONE;
        else
            return XML_Reader::XML_NONE;
    }

    bool Database::GetStartConditions(StartConditionsPrototype &p_proto)
    {
        std::string t_sDir[] = {"StartConditions"};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return false;

        for (AttributeList::iterator t_AttrItr = t_DBitr->second.m_AttributeList.begin();
             t_AttrItr != t_DBitr->second.m_AttributeList.end(); ++t_AttrItr)
        {
            // MapID
            if (t_AttrItr->first == "MapID")
            {
                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                p_proto.m_uiMapID = t_AttrItr->second.uintVal;
            }
            // HeroeID
            else if (t_AttrItr->first == "HeroeID")
            {
                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                p_proto.m_uiHeroeID = t_AttrItr->second.uintVal;
            }
            // X starting pos
            else if (t_AttrItr->first == "StartPosX")
            {
                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                p_proto.m_uiStartPos.x = t_AttrItr->second.uintVal;
            }
            // Y starting pos
            else if (t_AttrItr->first == "StartPosY")
            {
                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                p_proto.m_uiStartPos.y = t_AttrItr->second.uintVal;
            }
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
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Object");
             t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Object"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                // check ID
                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                if (t_AttrItr->second.uintVal == p_uiID)
                {
                    // store type before other
                    t_AttrItr = t_DBitr2->second.m_AttributeList.find("Type");
                    if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                        return NULL;
                    VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    t_proto.m_uiType = t_AttrItr->second.uiVal;

                    for (t_AttrItr = t_DBitr2->second.m_AttributeList.begin();
                    t_AttrItr != t_DBitr2->second.m_AttributeList.end(); ++t_AttrItr)
                    {
                        if (t_AttrItr->first == "ID")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            t_proto.m_uiID = t_AttrItr->second.uiVal;
                        }
                        else if (t_AttrItr->first == "Type")
                            continue;

                        else if (t_AttrItr->first == "TextureID")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            t_proto.m_uiTextureID = t_AttrItr->second.uiVal;
                        }
                        else if (t_AttrItr->first == "AniFrequency")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiAnimationFrequency = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "MoveSpeed")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiMoveSpeed = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "HP_min")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiHPmin = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "HP_max")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiHPmax = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "Level_min")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiLevelMin = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "Level_max")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiType)
                            {
                            case OBJECT_TYPE_NPC:
                                t_proto.ObjectType.NPC.m_uiLevelMax = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
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
        if (m_SpriteDB.find(p_sType) != m_SpriteDB.end() && m_SpriteDB.find(p_sType)->second.find(p_uiID) != m_SpriteDB.find(p_sType)->second.end())
                return &m_SpriteDB.find(p_sType)->second.find(p_uiID)->second;

        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return NULL;

        AttributeList::iterator t_AttrItr;
        SpritePrototype t_proto;
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("Sprite");
             t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "Sprite"; ++t_DBitr2)
        {
            if (t_DBitr2->second.HasAttributes())
            {
                t_AttrItr = t_DBitr2->second.m_AttributeList.find("ID");
                if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                    continue;

                // check ID
                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                if (t_AttrItr->second.uintVal == p_uiID)
                {
                    // store type before other
                    t_AttrItr = t_DBitr2->second.m_AttributeList.find("Type");
                    if (t_AttrItr == t_DBitr2->second.m_AttributeList.end())
                        return NULL;
                    VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    t_proto.m_uiSpriteType = t_AttrItr->second.uiVal;

                    for (t_AttrItr = t_DBitr2->second.m_AttributeList.begin();
                    t_AttrItr != t_DBitr2->second.m_AttributeList.end(); ++t_AttrItr)
                    {
                        if (t_AttrItr->first == "ID")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            t_proto.m_uiID = t_AttrItr->second.uiVal;
                        }
                        else if (t_AttrItr->first == "FileName")
                            t_proto.m_sFileName = bstr_t(t_AttrItr->second);

                        else if (t_AttrItr->first == "Type")
                            continue;

                        else if (t_AttrItr->first == "transparent_color")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UI4);
                            t_proto.m_transparentColor = t_AttrItr->second.ulVal;
                        }
                        else if (t_AttrItr->first == "spritesX")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                                // count of spritesX
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.AnimatedObject.m_uiSpritesX = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "spritesY")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                                // count of spritesX
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.AnimatedObject.m_uiSpritesY = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingXBegin")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingXBegin = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingYBegin")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingYBegin = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingXRange")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingXRange = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "boundingYRange")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                            case SPRITE_TYPE_OBJECT:
                            case SPRITE_TYPE_ANIMATED_OBJECT:
                                t_proto.Type.Object.m_uiBoundingYRange = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "passability")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                                // passability
                            case SPRITE_TYPE_TILE:
                            case SPRITE_TYPE_AUTOTILE:
                                t_proto.Type.Tile.m_uiPassable = t_AttrItr->second.uintVal;
                                break;
                            default:
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "terraintype")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                                // terrain type
                            case SPRITE_TYPE_TILE:
                            case SPRITE_TYPE_AUTOTILE:
                                t_proto.Type.Tile.m_uiTerrainType = t_AttrItr->second.uintVal;
                                break;
                            }
                        }
                        else if (t_AttrItr->first == "BorderSize")
                        {
                            VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                            switch(t_proto.m_uiSpriteType)
                            {
                                // border size
                            case SPRITE_TYPE_TEXTBOX:
                                t_proto.Type.Textbox.m_uiBorderSize = t_AttrItr->second.uintVal;
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
        for (ChildList::iterator t_DBitr2 = t_DBitr->second.m_ChildList.find("SpritePath");
             t_DBitr2 != t_DBitr->second.m_ChildList.end() && t_DBitr2->first == "SpritePath"; ++t_DBitr2)
        {
            for (t_AttrItr = t_DBitr2->second.m_AttributeList.begin();
            t_AttrItr != t_DBitr2->second.m_AttributeList.end(); ++t_AttrItr)
            {
                if (t_AttrItr->first == "ID")
                {
                    VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                    t_uiID = t_AttrItr->second.uintVal;
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
        p_lTextureNames.clear();
        std::string t_sDir[] = {"SpriteDatabase", p_sType};
        std::list<std::string> t_DirList(t_sDir, t_sDir + sizeof(t_sDir) / sizeof(std::string));
        ChildList::iterator t_DBitr;
        if (!ChangeDBdir(t_DirList, t_DBitr))
            return;

        std::string t_sName;
        AttributeList::iterator t_AttrItr;
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

                VariantChangeType(&t_AttrItr->second, &t_AttrItr->second, VARIANT_NOUSEROVERRIDE, VT_UINT);
                
                p_lTextureNames.insert(std::make_pair(t_AttrItr->second.uiVal, t_sName));
            }
        }
    }
};