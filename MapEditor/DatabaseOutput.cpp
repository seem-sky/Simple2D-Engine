#include "DatabaseOutput.h"
#include <XML_Writer.h>

using namespace XML;

namespace DATABASE
{
    DatabaseOutput::DatabaseOutput(void) : m_pWriter(NULL), TSingleton()
    {
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabaseOutput : ";
    }

    DatabaseOutput::~DatabaseOutput(void)
    {
        KillXMLThread();
    }

    void DatabaseOutput::ClearOutput()
    {
        m_ChangedSprites.clear();
        m_ChangedObjects.clear();
    }

    void DatabaseOutput::ChangeSpritePrototype(std::string p_sType, SpritePrototype &p_ChangedProto)
    {
        // find type or create it
        SpriteList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
        {
            std::map<UINT, SpritePrototype> t_NewMap;
            m_ChangedSprites.insert(std::make_pair(p_sType, t_NewMap));
            t_TypeItr = m_ChangedSprites.find(p_sType);
        }

        // find prototype and overwrite it, or create it
        std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.find(p_ChangedProto.m_uiID);
        if (t_SpriteItr == t_TypeItr->second.end())
            t_TypeItr->second.insert(std::make_pair(p_ChangedProto.m_uiID, p_ChangedProto));
        else
            t_SpriteItr->second = p_ChangedProto;
    }

    SpritePrototype* DatabaseOutput::GetSpritePrototype(std::string p_sType, UINT p_uiID)
    {
        // find type
        SpriteList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
            return NULL;

        // find sprite
        std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.find(p_uiID);
        if (t_SpriteItr == t_TypeItr->second.end())
            return NULL;

        return &t_SpriteItr->second;
    }

    void DatabaseOutput::DeleteSpritePrototype(std::string p_sType, SpritePrototype &p_DelProto)
    {
        ChangeSpritePrototype(p_sType, p_DelProto);
        if (SpritePrototype *t_pProto = GetSpritePrototype(p_sType, p_DelProto.m_uiID))
            t_pProto->m_uiID = 0;
    }

    bool DatabaseOutput::IsSpritePrototypeDeleted(std::string p_sType, UINT p_uiID)
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
        XML_WriteData *t_DBChanges = new XML_WriteData();

        // parse sprite changes
        if (!m_ChangedSprites.empty())
        {
            XML_WriteData t_NewChildren;
            t_NewChildren.AddChild("SpriteDatabase", XML_WriteData());
            if (XML_WriteData *p_pSpriteDB = t_NewChildren.GetChild("SpriteDatabase"))
            {
                // iterate through type
                for (SpriteList::iterator t_TypeItr = m_ChangedSprites.begin(); t_TypeItr != m_ChangedSprites.end(); ++t_TypeItr)
                {
                    XML_WriteData *p_pSpriteType = p_pSpriteDB->GetChild(t_TypeItr->first);
                    if (!p_pSpriteType)
                    {
                        p_pSpriteDB->AddChild(t_TypeItr->first, XML_WriteData());
                        p_pSpriteType = p_pSpriteDB->GetChild(t_TypeItr->first);
                    }

                    // iterate through prototypes
                    for (std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.begin(); t_SpriteItr != t_TypeItr->second.end(); ++t_SpriteItr)
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
                            CComVariant t_Value;
                            t_Value.uintVal = t_SpriteItr->first;
                            t_NewElement.AddAttribute("ID", t_Value);
                            t_NewElement.SetWriteState(XML_WRITE_DELETE);
                            p_pSpriteType->AddChild("Sprite", t_NewElement);
                        }
                    }
                }
            }
            t_DBChanges->AddChild("Database", t_NewChildren);
        }

        // parse object changes
        if (!m_ChangedObjects.empty())
        {
            XML_WriteData t_NewChildren;
            t_NewChildren.AddChild("ObjectDatabase", XML_WriteData());

            XML_WriteData *t_pObjDB = t_NewChildren.GetChild("ObjectDatabase");
            if (t_pObjDB)
            {
                // iterate through prototypes
                for (std::map<UINT, ObjectPrototype>::iterator t_ObjectItr = m_ChangedObjects.begin(); t_ObjectItr != m_ChangedObjects.end(); ++t_ObjectItr)
                {
                    XML_WriteData t_NewElement;
                    // parse added or changed sprites
                    if (t_ObjectItr->second.m_uiID != 0)
                    {
                        if (ParseObjectChange(&t_NewElement, &t_ObjectItr->second))
                            t_pObjDB->AddChild("Object", t_NewElement);
                    }
                    // deleted sprite
                    else
                    {
                        CComVariant t_Value;
                        t_Value.uintVal = t_ObjectItr->first;
                        t_NewElement.AddAttribute("ID", t_Value);
                        t_NewElement.SetWriteState(XML_WRITE_DELETE);
                        t_pObjDB->AddChild("Object", t_NewElement);
                    }
                }
            }

            XML_WriteData *t_pDB = t_DBChanges->GetChild("Database");
            if (t_pDB)
            {
                XML_WriteData *t_ObjDB = t_NewChildren.GetChild("ObjectDatabase");
                if (t_ObjDB)
                    t_pDB->AddChild("ObjectDatabase", *t_ObjDB);
            }
            else
                t_DBChanges->AddChild("Database", t_NewChildren);
        }

        m_pWriter = new XML_Writer(p_sFileName, t_DBChanges);
        ClearOutput();
    }

    bool DatabaseOutput::ParseObjectChange(XML::XML_WriteData *p_pElement, const ObjectPrototype *p_pProto)
    {
        if (!p_pElement || !p_pProto)
            return false;

        // filename
        p_pElement->AddAttribute("ObjectName", (LPCOLESTR)_bstr_t(p_pProto->m_sName.c_str()));

        // ID
        p_pElement->AddAttribute("ID", p_pProto->m_uiID);

        // Type
        p_pElement->AddAttribute("Type", p_pProto->m_uiType);

        // Texture ID
        p_pElement->AddAttribute("TextureID", p_pProto->m_uiTextureID);

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
        if (!p_pProto->m_ObjectBoolList.empty() || !p_pProto->m_ObjectIntegerList.empty() || !p_pProto->m_ObjectFloatList.empty() || !p_pProto->m_ObjectStringList.empty())
        {
            if (!p_pElement->HasChild("Variables"))
                p_pElement->AddChild("Variables", XML_WriteData(XML::XML_WRITE_CHANGE));

            if (XML_WriteData* t_pVariables = (XML_WriteData*)p_pElement->GetChild("Variables"))
            {
                for (ObjectBoolList::const_iterator t_Itr = p_pProto->m_ObjectBoolList.begin(); t_Itr != p_pProto->m_ObjectBoolList.end(); ++t_Itr)
                {
                    XML_WriteData t_NewVariable(XML::XML_WRITE_CHANGE);
                    // 0 == delete mode
                    if (t_Itr->second.m_uiID != 0)
                    {
                        t_NewVariable.AddAttribute("name", (LPCOLESTR)_bstr_t(t_Itr->second.m_sName.c_str()));
                        t_NewVariable.AddAttribute("ID", t_Itr->second.m_uiID);
                        t_NewVariable.AddAttribute("value", t_Itr->second.m_Value);
                    }
                    else
                    {
                        t_NewVariable.SetWriteState(XML_WRITE_DELETE);
                        t_NewVariable.AddAttribute("ID", t_Itr->first);
                    }

                    t_pVariables->AddChild("bool", t_NewVariable);
                }

                for (ObjectIntegerList::const_iterator t_Itr = p_pProto->m_ObjectIntegerList.begin(); t_Itr != p_pProto->m_ObjectIntegerList.end(); ++t_Itr)
                {
                    XML_WriteData t_NewVariable(XML::XML_WRITE_CHANGE);
                    // 0 == delete mode
                    if (t_Itr->second.m_uiID != 0)
                    {
                        t_NewVariable.AddAttribute("name", (LPCOLESTR)_bstr_t(t_Itr->second.m_sName.c_str()));
                        t_NewVariable.AddAttribute("ID", t_Itr->second.m_uiID);
                        t_NewVariable.AddAttribute("value", t_Itr->second.m_Value);
                    }
                    else
                    {
                        t_NewVariable.SetWriteState(XML_WRITE_DELETE);
                        t_NewVariable.AddAttribute("ID", t_Itr->first);
                    }

                    t_pVariables->AddChild("integer", t_NewVariable);
                }

                for (ObjectFloatList::const_iterator t_Itr = p_pProto->m_ObjectFloatList.begin(); t_Itr != p_pProto->m_ObjectFloatList.end(); ++t_Itr)
                {
                    XML_WriteData t_NewVariable(XML::XML_WRITE_CHANGE);
                    // 0 == delete mode
                    if (t_Itr->second.m_uiID != 0)
                    {
                        t_NewVariable.AddAttribute("name", (LPCOLESTR)_bstr_t(t_Itr->second.m_sName.c_str()));
                        t_NewVariable.AddAttribute("ID", t_Itr->second.m_uiID);
                        t_NewVariable.AddAttribute("value", t_Itr->second.m_Value);
                    }
                    else
                    {
                        t_NewVariable.SetWriteState(XML_WRITE_DELETE);
                        t_NewVariable.AddAttribute("ID", t_Itr->first);
                    }

                    t_pVariables->AddChild("float", t_NewVariable);
                }

                for (ObjectStringList::const_iterator t_Itr = p_pProto->m_ObjectStringList.begin(); t_Itr != p_pProto->m_ObjectStringList.end(); ++t_Itr)
                {
                    XML_WriteData t_NewVariable(XML::XML_WRITE_CHANGE);
                    // 0 == delete mode
                    if (t_Itr->second.m_uiID != 0)
                    {
                        t_NewVariable.AddAttribute("name", (LPCOLESTR)_bstr_t(t_Itr->second.m_sName.c_str()));
                        t_NewVariable.AddAttribute("ID", t_Itr->second.m_uiID);
                        t_NewVariable.AddAttribute("value", (LPCOLESTR)_bstr_t(t_Itr->second.m_Value.c_str()));
                    }
                    else
                    {
                        t_NewVariable.SetWriteState(XML_WRITE_DELETE);
                        t_NewVariable.AddAttribute("ID", t_Itr->first);
                    }

                    t_pVariables->AddChild("string", t_NewVariable);
                }
            }
        }

        // set change flag
        p_pElement->SetWriteState(XML_WRITE_CHANGE);

        return true;
    }

    bool DatabaseOutput::ParseSpriteChange(XML_WriteData *p_pElement, const SpritePrototype *p_pProto)
    {
        if (!p_pElement || !p_pProto)
            return false;

        // filename
        p_pElement->AddAttribute("FileName", (LPCOLESTR)_bstr_t(p_pProto->m_sFileName.c_str()));

        // ID
        p_pElement->AddAttribute("ID", p_pProto->m_uiID);

        // Type
        p_pElement->AddAttribute("Type", p_pProto->m_uiSpriteType);

        // transparent_color
        p_pElement->AddAttribute("transparent_color", (LPCOLESTR)_bstr_t(p_pProto->m_sTransparentColor.c_str()));

        // type specific stuff
        switch (p_pProto->m_uiSpriteType)
        {
        case SPRITE_TYPE_TILE:
            // passability
            p_pElement->AddAttribute("passability", p_pProto->Type.Tile.m_uiPassable);

            // terraintype
            p_pElement->AddAttribute("terraintype", p_pProto->Type.Tile.m_uiTerrainType);

            // is autotile
            p_pElement->AddAttribute("auto_tile", (UINT)p_pProto->Type.Tile.m_bAutotile);
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

    void DatabaseOutput::GetTextureNames(std::string p_sType, std::map<UINT, std::string> &p_lTextureNames)
    {
        // find type
        SpriteList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
            return;

        std::map<UINT, std::string>::iterator t_NameItr;
        for (std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.begin(); t_SpriteItr != t_TypeItr->second.end(); ++t_SpriteItr)
        {
            t_NameItr = p_lTextureNames.find(t_SpriteItr->first);
            if (t_NameItr == p_lTextureNames.end())
                p_lTextureNames.insert(std::make_pair(t_SpriteItr->first, t_SpriteItr->second.m_sFileName));

            else
                t_NameItr->second = t_SpriteItr->second.m_sFileName;
        }
    }

    XML_STATE DatabaseOutput::GetDBState()
    {
        // progress XML writer
        XML_STATE t_State = XML_NONE;
        if (m_pWriter)
        {
            t_State = m_pWriter->GetWriterState();
            switch(t_State)
            {
            case XML_IN_PROGRESS:
                break;
            case XML_DONE:
                BASIC_LOG(m_sLogLocationName + "Database save complete.");
            default:
                KillXMLThread();
                break;
            }
        }
        return t_State;
    }

    ObjectPrototype* DatabaseOutput::GetObjectPrototype(unsigned int p_uiID)
    {
        ObjectList::iterator t_ObjItr = m_ChangedObjects.find(p_uiID);
        if (t_ObjItr != m_ChangedObjects.end())
            return &t_ObjItr->second;

        return NULL;
    }

    void DatabaseOutput::ChangeObjectPrototype(ObjectPrototype &p_ChangedProto)
    {
        // find prototype and overwrite it, or create it
        ObjectList::iterator t_ObjItr = m_ChangedObjects.find(p_ChangedProto.m_uiID);
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

    bool DatabaseOutput::IsObjectPrototypeDeleted(UINT p_uiID)
    {
        if (ObjectPrototype *t_pProto = GetObjectPrototype(p_uiID))
            if (t_pProto->m_uiID == 0)
                return true;

        return false;
    }

    void DatabaseOutput::GetObjectNames(std::map<UINT, std::string> &p_lObjectNames)
    {
        std::map<UINT, std::string>::iterator t_NameItr;
        for (ObjectList::iterator t_ObjectItr = m_ChangedObjects.begin(); t_ObjectItr != m_ChangedObjects.end(); ++t_ObjectItr)
        {
            t_NameItr = p_lObjectNames.find(t_ObjectItr->first);
            if (t_NameItr == p_lObjectNames.end())
                p_lObjectNames.insert(std::make_pair(t_ObjectItr->first, t_ObjectItr->second.m_sName));

            else
                t_NameItr->second = t_ObjectItr->second.m_sName;
        }
    }

    UINT DatabaseOutput::AddNewCustomObjectVariable(UINT p_uiObjectID, CUSTOM_VARIABLE_TYPE p_Type)
    {
        ObjectPrototype t_Proto;
        if (GetObjectPrototype(p_uiObjectID))
            t_Proto = *GetObjectPrototype(p_uiObjectID);
        else
        {
            if (Database *t_pDB = Database::Get())
                if (t_pDB->GetObjectPrototype(p_uiObjectID))
                    t_Proto = *t_pDB->GetObjectPrototype(p_uiObjectID);
        }

        UINT t_uiIDCheck = 1;
        bool t_bSuccess = false;
        switch(p_Type)
        {
        case VARIABLE_BOOL:
            for (ObjectBoolList::iterator t_Itr = t_Proto.m_ObjectBoolList.begin(); t_Itr != t_Proto.m_ObjectBoolList.end(); ++t_Itr)
            {
                // 0 == delete mode
                if (t_uiIDCheck < t_Itr->first)
                {
                    TObjectVariable<bool> t_Variable;
                    t_Variable.m_uiID = t_uiIDCheck;
                    t_Proto.m_ObjectBoolList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                    t_bSuccess = true;
                    break;
                }
                else if (t_Itr->second.m_uiID == 0)
                {
                    t_Itr->second.m_uiID = t_uiIDCheck;
                    t_bSuccess = true;
                    break;
                }
                ++t_uiIDCheck;
            }

            if (!t_bSuccess)
            {
                TObjectVariable<bool> t_Variable;
                t_Variable.m_uiID = t_uiIDCheck;
                t_Proto.m_ObjectBoolList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                t_bSuccess = true;
            }

            break;
        case VARIABLE_INT:
            for (ObjectIntegerList::iterator t_Itr = t_Proto.m_ObjectIntegerList.begin(); t_Itr != t_Proto.m_ObjectIntegerList.end(); ++t_Itr)
            {
                // 0 == delete mode
                if (t_uiIDCheck < t_Itr->first)
                {
                    TObjectVariable<int> t_Variable;
                    t_Variable.m_uiID = t_uiIDCheck;
                    t_Proto.m_ObjectIntegerList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                    t_bSuccess = true;
                    break;
                }
                else if (t_Itr->second.m_uiID == 0)
                {
                    t_Itr->second.m_uiID = t_uiIDCheck;
                    t_bSuccess = true;
                    break;
                }

                ++t_uiIDCheck;
            }

            if (!t_bSuccess)
            {
                TObjectVariable<int> t_Variable;
                t_Variable.m_uiID = t_uiIDCheck;
                t_Proto.m_ObjectIntegerList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                t_bSuccess = true;
            }
            break;
        case VARIABLE_FLOAT:
            for (ObjectFloatList::iterator t_Itr = t_Proto.m_ObjectFloatList.begin(); t_Itr != t_Proto.m_ObjectFloatList.end(); ++t_Itr)
            {
                // 0 == delete mode
                if (t_uiIDCheck < t_Itr->first)
                {
                    TObjectVariable<float> t_Variable;
                    t_Variable.m_uiID = t_uiIDCheck;
                    t_Proto.m_ObjectFloatList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                    t_bSuccess = true;
                    break;
                }
                else if (t_Itr->second.m_uiID == 0)
                {
                    t_Itr->second.m_uiID = t_uiIDCheck;
                    t_bSuccess = true;
                    break;
                }

                ++t_uiIDCheck;
            }

            if (!t_bSuccess)
            {
                TObjectVariable<float> t_Variable;
                t_Variable.m_uiID = t_uiIDCheck;
                t_Proto.m_ObjectFloatList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                t_bSuccess = true;
            }
            break;
        case VARIABLE_STRING:
            for (ObjectStringList::iterator t_Itr = t_Proto.m_ObjectStringList.begin(); t_Itr != t_Proto.m_ObjectStringList.end(); ++t_Itr)
            {
                // 0 == delete mode
                if (t_uiIDCheck < t_Itr->first)
                {
                    TObjectVariable<std::string> t_Variable;
                    t_Variable.m_uiID = t_uiIDCheck;
                    t_Proto.m_ObjectStringList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                    t_bSuccess = true;
                    break;
                }
                else if (t_Itr->second.m_uiID == 0)
                {
                    t_Itr->second.m_uiID = t_uiIDCheck;
                    t_bSuccess = true;
                    break;
                }

                ++t_uiIDCheck;
            }

            if (!t_bSuccess)
            {
                TObjectVariable<std::string> t_Variable;
                t_Variable.m_uiID = t_uiIDCheck;
                t_Proto.m_ObjectStringList.insert(std::make_pair(t_uiIDCheck, t_Variable));
                t_bSuccess = true;
            }
            break;
        default:
            break;
        }

        if (t_bSuccess)
        {
            ChangeObjectPrototype(t_Proto);
            return t_uiIDCheck;
        }

        return 0;
    }

    bool DatabaseOutput::DeleteCustomObjectVariable(UINT p_uiObjectID, CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiVariableID)
    {
        ObjectPrototype t_Proto;
        if (GetObjectPrototype(p_uiObjectID))
            t_Proto = *GetObjectPrototype(p_uiObjectID);
        else
        {
            if (Database *t_pDB = Database::Get())
                if (t_pDB->GetObjectPrototype(p_uiObjectID))
                    t_Proto = *t_pDB->GetObjectPrototype(p_uiObjectID);
        }

        bool t_bSuccess = false;
        switch(p_Type)
        {
        case VARIABLE_BOOL:
            {
                ObjectBoolList::iterator t_Itr = t_Proto.m_ObjectBoolList.find(p_uiVariableID);
                if (t_Itr != t_Proto.m_ObjectBoolList.end())
                {
                    t_Itr->second.m_uiID = 0;
                    t_bSuccess = true;
                }

                break;
            }
        case VARIABLE_INT:
            {
                ObjectIntegerList::iterator t_Itr = t_Proto.m_ObjectIntegerList.find(p_uiVariableID);
                if (t_Itr != t_Proto.m_ObjectIntegerList.end())
                {
                    t_Itr->second.m_uiID = 0;
                    t_bSuccess = true;
                }

                break;
            }
            break;
        case VARIABLE_FLOAT:
            {
                ObjectFloatList::iterator t_Itr = t_Proto.m_ObjectFloatList.find(p_uiVariableID);
                if (t_Itr != t_Proto.m_ObjectFloatList.end())
                {
                    t_Itr->second.m_uiID = 0;
                    t_bSuccess = true;
                }

                break;
            }
            break;
        case VARIABLE_STRING:
            {
                ObjectStringList::iterator t_Itr = t_Proto.m_ObjectStringList.find(p_uiVariableID);
                if (t_Itr != t_Proto.m_ObjectStringList.end())
                {
                    t_Itr->second.m_uiID = 0;
                    t_bSuccess = true;
                }

                break;
            }
            break;
        default:
            break;
        }

        if (t_bSuccess)
            ChangeObjectPrototype(t_Proto);

        return t_bSuccess;
    }
}