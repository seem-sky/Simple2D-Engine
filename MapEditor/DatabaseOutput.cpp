#include "DatabaseOutput.h"
#include <XML_Writer.h>

using namespace XML;

namespace DATABASE
{
    DatabaseOutput::DatabaseOutput(void) : m_pWriter(NULL), TSingleton()
    {
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabaseOutput : ";
    }

    void DatabaseOutput::ClearOutput()
    {
        m_ChangedSprites.clear();
        m_ChangedObjects.clear();
        m_Variables.Clear();
    }

    void DatabaseOutput::ChangeSpritePrototype(std::string p_sType, SpritePrototype &p_ChangedProto)
    {
        // find type or create it
        SpriteTypeList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
        {
            std::map<uint32, SpritePrototype> t_NewMap;
            m_ChangedSprites.insert(std::make_pair(p_sType, t_NewMap));
            t_TypeItr = m_ChangedSprites.find(p_sType);
        }

        // find prototype and overwrite it, or create it
        std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.find(p_ChangedProto.m_uiID);
        if (t_SpriteItr == t_TypeItr->second.end())
            t_TypeItr->second.insert(std::make_pair(p_ChangedProto.m_uiID, p_ChangedProto));
        else
            t_SpriteItr->second = p_ChangedProto;
    }

    SpritePrototype* DatabaseOutput::GetSpritePrototype(std::string p_sType, uint32 p_uiID)
    {
        // find type
        SpriteTypeList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
            return NULL;

        // find sprite
        std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.find(p_uiID);
        if (t_SpriteItr == t_TypeItr->second.end())
            return NULL;

        return &t_SpriteItr->second;
    }

    const SpritePrototype* DatabaseOutput::GetLatestSpritePrototype(std::string p_sType, uint32 p_uiID)
    {
        const SpritePrototype *pProto = NULL;
        if (pProto = GetSpritePrototype(p_sType, p_uiID))
            return pProto;
        if (Database *pDB = Database::Get())
            return pDB->GetSpritePrototype(p_sType, p_uiID);
        return pProto;
    }

    void DatabaseOutput::DeleteSpritePrototype(std::string p_sType, SpritePrototype &p_DelProto)
    {
        ChangeSpritePrototype(p_sType, p_DelProto);
        if (SpritePrototype *t_pProto = GetSpritePrototype(p_sType, p_DelProto.m_uiID))
            t_pProto->m_uiID = 0;
    }

    bool DatabaseOutput::IsSpritePrototypeDeleted(std::string p_sType, uint32 p_uiID)
    {
        if (SpritePrototype *t_pProto = GetSpritePrototype(p_sType, p_uiID))
            if (t_pProto->m_uiID == 0)
                return true;

        return false;
    }

    void DatabaseOutput::SaveChangesTo(std::string p_sFileName)
    {
        if (m_pWriter)
            return;

        XML_WriteData t_DBChanges;
        XML_WriteData t_DB;

        // parse sprite changes
        if (!m_ChangedSprites.empty())
        {
            XML_WriteData t_SpriteDB;
            // iterate through type
            for (SpriteTypeList::iterator t_TypeItr = m_ChangedSprites.begin(); t_TypeItr != m_ChangedSprites.end(); ++t_TypeItr)
            {
                XML_WriteData *p_pSpriteType = t_SpriteDB.GetChild(t_TypeItr->first);
                if (!p_pSpriteType)
                {
                    t_SpriteDB.AddChild(t_TypeItr->first, XML_WriteData());
                    p_pSpriteType = t_SpriteDB.GetChild(t_TypeItr->first);
                }

                // iterate through prototypes
                for (std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.begin(); t_SpriteItr != t_TypeItr->second.end(); ++t_SpriteItr)
                {
                    XML_WriteData t_NewElement;
                    // parse added or changed sprites
                    if (t_SpriteItr->second.m_uiID != 0)
                    {
                        if (ParseSpriteChange(&t_NewElement, &t_SpriteItr->second))
                            p_pSpriteType->AddChild("Sprite", t_NewElement);
                    }
                    // deleted sprite
                    else
                    {
                        t_NewElement.AddAttribute("ID", t_SpriteItr->first);
                        t_NewElement.SetWriteState(XML_WRITE_DELETE);
                        p_pSpriteType->AddChild("Sprite", t_NewElement);
                    }
                }
            }
            t_DB.AddChild("SpriteDatabase", t_SpriteDB);
        }

        // parse object changes
        if (!m_ChangedObjects.empty())
        {
            XML_WriteData t_ObjDB;
            // iterate through prototypes
            for (std::map<uint32, ObjectPrototype>::iterator t_ObjectItr = m_ChangedObjects.begin(); t_ObjectItr != m_ChangedObjects.end(); ++t_ObjectItr)
            {
                XML_WriteData t_NewElement;
                // parse added or changed sprites
                if (t_ObjectItr->second.m_uiID != 0)
                {
                    if (ParseObjectChange(&t_NewElement, &t_ObjectItr->second))
                        t_ObjDB.AddChild("Object", t_NewElement);
                }
                // deleted sprite
                else
                {
                    t_NewElement.AddAttribute("ID", t_ObjectItr->first);
                    t_NewElement.SetWriteState(XML_WRITE_DELETE);
                    t_ObjDB.AddChild("Object", t_NewElement);
                }
            }
            t_DB.AddChild("ObjectDatabase", t_ObjDB);
        }

        // get global variable changes
        XML_WriteData t_VariableDB(XML_WRITE_OVERWRITE);
        m_Variables.GetXMLData(t_VariableDB);
        if (t_VariableDB.HasChilds())
            t_DB.AddChild("GlobalVariables", t_VariableDB);

        t_DBChanges.AddChild("Database", t_DB);
        m_pWriter = new XML_Writer();
        m_pWriter->startWriting(p_sFileName, t_DBChanges);
        ClearOutput();
    }

    bool DatabaseOutput::ParseObjectChange(XML::XML_WriteData *p_pElement, const ObjectPrototype *p_pProto)
    {
        if (!p_pElement || !p_pProto)
            return false;

        // filename
        p_pElement->AddAttribute("ObjectName", (LPCSTR)p_pProto->m_sName.c_str());

        // ID
        p_pElement->AddAttribute("ID", p_pProto->m_uiID);

        // Type
        p_pElement->AddAttribute("Type", p_pProto->m_uiType);

        // Texture ID
        p_pElement->AddAttribute("TextureID", p_pProto->m_uiTextureID);

        // Parent ID
        if (p_pProto->GetParentList())
        {
            XML_WriteData t_Parents(XML_WRITE_OVERWRITE);
            for (IDList::const_iterator t_Itr = p_pProto->GetParentList()->begin(); t_Itr != p_pProto->GetParentList()->end(); ++t_Itr)
            {
                XML_WriteData t_NewParent(XML_WRITE_APPEND);
                t_NewParent.AddAttribute("ID", *t_Itr);
                t_Parents.AddChild("Parent", t_NewParent);
            }
            p_pElement->AddChild("Parents", t_Parents);
        }

        switch(p_pProto->m_uiType)
        {
        case OBJECT_TYPE_NPC:
            // animation frequency
            p_pElement->AddAttribute("AnimationFrequency", p_pProto->ObjectType.NPC.m_uiAnimationFrequency);

            // movement speed
            p_pElement->AddAttribute("MovementSpeed", p_pProto->ObjectType.NPC.m_uiMoveSpeed);

            // HP_min
            p_pElement->AddAttribute("HP_min", p_pProto->ObjectType.NPC.m_uiHPmin);

            // HP_max
            p_pElement->AddAttribute("HP_max", p_pProto->ObjectType.NPC.m_uiHPmax);
        case OBJECT_TYPE_MAP_OBJECT:
            break;
        default:
            break;
        }

        // parse custom variables
        if (!p_pElement->HasChild("Variables"))
            p_pElement->AddChild("Variables", XML_WriteData(XML_WRITE_OVERWRITE));

        if (XML_WriteData* t_pVariables = (XML_WriteData*)p_pElement->GetChild("Variables"))
            p_pProto->m_Variables.GetXMLData(*t_pVariables);

        // parse object scripts
        if (!p_pElement->HasChild("Scripts"))
            p_pElement->AddChild("Scripts", XML_WriteData(XML_WRITE_OVERWRITE));

        if (XML_WriteData* t_pScripts = (XML_WriteData*)p_pElement->GetChild("Scripts"))
            p_pProto->m_Scripts.GetXMLData(*t_pScripts);

        // set change flag
        p_pElement->SetWriteState(XML_WRITE_CHANGE);

        return true;
    }

    bool DatabaseOutput::ParseSpriteChange(XML_WriteData *p_pElement, const SpritePrototype *p_pProto)
    {
        if (!p_pElement || !p_pProto)
            return false;

        // filename
        p_pElement->AddAttribute("FileName", (LPCSTR)p_pProto->m_sFileName.c_str());

        // path
        p_pElement->AddAttribute("Path", (LPCSTR)p_pProto->m_sPath.c_str());

        // ID
        p_pElement->AddAttribute("ID", p_pProto->m_uiID);

        // Type
        p_pElement->AddAttribute("Type", p_pProto->m_uiSpriteType);

        // transparent_color
        std::string t_sColorString;
        p_pProto->m_TransparencyColor.getColorString(t_sColorString);
        p_pElement->AddAttribute("transparent_color", (LPCSTR)t_sColorString.c_str());

        // type specific stuff
        switch (p_pProto->m_uiSpriteType)
        {
        case SPRITE_TYPE_TILE:
            // passability
            p_pElement->AddAttribute("passability", p_pProto->Type.Tile.m_uiPassable);

            // terraintype
            p_pElement->AddAttribute("terraintype", p_pProto->Type.Tile.m_uiTerrainType);

            // is autotile
            p_pElement->AddAttribute("auto_tile", (uint32)p_pProto->Type.Tile.m_bAutotile);
        	break;

        case SPRITE_TYPE_ANIMATED_OBJECT:
            // columns
            p_pElement->AddAttribute("spritesX", p_pProto->Type.AnimatedObject.m_uiSpritesX);

            // rows
            p_pElement->AddAttribute("spritesY", p_pProto->Type.AnimatedObject.m_uiSpritesY);

        case SPRITE_TYPE_OBJECT:
            // bounding rect
            p_pElement->AddAttribute("boundingXBegin", p_pProto->Type.Object.m_uiBoundingXBegin);
            p_pElement->AddAttribute("boundingXRange", p_pProto->Type.Object.m_uiBoundingXRange);
            p_pElement->AddAttribute("boundingYBegin", p_pProto->Type.Object.m_uiBoundingYBegin);
            p_pElement->AddAttribute("boundingYRange", p_pProto->Type.Object.m_uiBoundingYRange);
            break;
        default:
            break;
        }

        // set change flag
        p_pElement->SetWriteState(XML_WRITE_CHANGE);

        return true;
    }

    void DatabaseOutput::GetTextureNames(std::string p_sType, std::map<uint32, std::string> &p_lTextureNames)
    {
        // find type
        SpriteTypeList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
            return;

        std::map<uint32, std::string>::iterator t_NameItr;
        for (std::map<uint32, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.begin(); t_SpriteItr != t_TypeItr->second.end(); ++t_SpriteItr)
        {
            t_NameItr = p_lTextureNames.find(t_SpriteItr->first);
            if (t_NameItr == p_lTextureNames.end())
                p_lTextureNames.insert(std::make_pair(t_SpriteItr->first, t_SpriteItr->second.m_sFileName));

            else
                t_NameItr->second = t_SpriteItr->second.m_sFileName;
        }
    }

    ThreadState DatabaseOutput::GetDBState()
    {
        // progress XML writer
        ThreadState t_State = THREAD_NONE;
        if (m_pWriter)
        {
            switch(t_State)
            {
            case THREAD_IN_PROGRESS:
                break;
            case THREAD_DONE:
                BASIC_LOG(m_sLogLocationName + "Database save complete.");
                break;
            }
        }
        return t_State;
    }

    ObjectPrototype* DatabaseOutput::GetObjectPrototype(uint32 p_uiID)
    {
        ObjectPrototypeMap::iterator t_ObjItr = m_ChangedObjects.find(p_uiID);
        if (t_ObjItr != m_ChangedObjects.end())
            return &t_ObjItr->second;

        return NULL;
    }

    void DatabaseOutput::ChangeObjectPrototype(ObjectPrototype &p_ChangedProto)
    {
        // find prototype and overwrite it, or create it
        ObjectPrototypeMap::iterator t_ObjItr = m_ChangedObjects.find(p_ChangedProto.m_uiID);
        if (t_ObjItr == m_ChangedObjects.end())
            m_ChangedObjects.insert(std::make_pair(p_ChangedProto.m_uiID, p_ChangedProto));
        else
            t_ObjItr->second = p_ChangedProto;
    }

    void DatabaseOutput::DeleteObjectPrototype(ObjectPrototype &p_DelProto)
    {
        ChangeObjectPrototype(p_DelProto);
        if (ObjectPrototype *t_pProto = GetObjectPrototype(p_DelProto.m_uiID))
            t_pProto->m_uiID = 0;
    }

    bool DatabaseOutput::IsObjectPrototypeDeleted(uint32 p_uiID)
    {
        if (ObjectPrototype *t_pProto = GetObjectPrototype(p_uiID))
            if (t_pProto->m_uiID == 0)
                return true;

        return false;
    }

    void DatabaseOutput::GetObjectNames(std::map<uint32, std::string> &p_lObjectNames)
    {
        std::map<uint32, std::string>::iterator t_NameItr;
        for (ObjectPrototypeMap::iterator t_ObjectItr = m_ChangedObjects.begin(); t_ObjectItr != m_ChangedObjects.end(); ++t_ObjectItr)
        {
            t_NameItr = p_lObjectNames.find(t_ObjectItr->first);
            if (t_NameItr == p_lObjectNames.end())
                p_lObjectNames.insert(std::make_pair(t_ObjectItr->first, t_ObjectItr->second.m_sName));

            else
                t_NameItr->second = t_ObjectItr->second.m_sName;
        }
    }

    ObjectPrototype* DatabaseOutput::GetLatestObjectPrototype(uint32 p_uiID)
    {
        ObjectPrototype *t_pResult = GetObjectPrototype(p_uiID);
        if (t_pResult)
            return t_pResult;
            
        if (Database *t_pDB = Database::Get())
        {
            if (const ObjectPrototype *t_pProto = t_pDB->GetObjectPrototype(p_uiID))
                m_ChangedObjects.insert(std::make_pair(p_uiID, *t_pProto));

            else
            {
                ObjectPrototype t_Proto;
                t_Proto.m_uiID = p_uiID;
                m_ChangedObjects.insert(std::make_pair(p_uiID, t_Proto));
            }

            ObjectPrototype *t_pResult = GetObjectPrototype(p_uiID);
            if (t_pResult)
                return t_pResult;
        }

        return NULL;
    }

    IDList DatabaseOutput::GetAllParents(const Prototype *p_pProto)
    {
        IDList t_uiParentIDList;
        if (!p_pProto)
            return t_uiParentIDList;

        t_uiParentIDList = *p_pProto->GetParentList();
        for (IDList::const_iterator t_Itr = p_pProto->GetParentList()->begin(); t_Itr != p_pProto->GetParentList()->end(); ++t_Itr)
        {
            if (const ObjectPrototype *t_pParent = GetLatestObjectPrototype(*t_Itr))
            {
                IDList t_uiTempIDList = GetAllParents(t_pParent);
                t_uiParentIDList.insert(t_uiTempIDList.begin(), t_uiTempIDList.end());
            }
        }

        return t_uiParentIDList;
    }

    const VariableHolder* DatabaseOutput::GetVariableHolderFromParent(const DATABASE::PrototypeType p_ProtoType, const uint32 p_uiParentID)
    {
        switch(p_ProtoType)
        {
            case PROTOTYPE_TYPE_OBJECT:
                if (const ObjectPrototype *t_pParent = GetLatestObjectPrototype(p_uiParentID))
                    return &t_pParent->m_Variables;
                break;
            case PROTOTYPE_TYPE_NONE:
            default:
                break;
        }

        return NULL;
    }
}